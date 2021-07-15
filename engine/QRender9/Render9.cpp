#include "Base_include.h"
#include "../QRender/QRender.h"
#include "Render9.h"
#include "IRenderable9.h"

#include <Windows.h>
#include <d3dx9.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Render9::Render9()
{
	NeedToClose = false;

	CreateWnd();
	CreateDirectX();

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);
	SetForegroundWindow(hWnd);
}

Render9::~Render9()
{
}

void Render9::SetPreRender(PreRenderFunction pre_function)
{
	preFunction = pre_function;
}

void Render9::SetRender(RenderFunction render_function)
{
	//renderFunction = render_function;
}

void Render9::SetPostRender(PostRenderFunction post_function)
{
	postFunction = post_function;
}

void Render9::SetShutdown(ShutdownFunction shutdown_function)
{
	shutdownFunction = shutdown_function;
}

void Render9::Render()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			RenderIn();

			if (NeedToClose)
			{
				Shutdown();
				break;
			}
		}
	}
}

#include <cstdarg>

IRenderable* Render9::CreateRenderable(RenderableType type, ...)
{
	switch (type)
	{
	case RenderableType::NONE:
	{
		return nullptr;
	}
	break;
	case RenderableType::SPRITE:
	{
		IRenderable9* sprite = new IRenderable9();
		sprite->_type = RenderableType::SPRITE;

		va_list args;
		va_start(args, type);

		const char* path = va_arg(args, const char*);
		int x = va_arg(args, int);
		int y = va_arg(args, int);
		int w = va_arg(args, int);
		int h = va_arg(args, int);
		//const char* vert = va_arg(args, const char*);
		//const char* frag = va_arg(args, const char*);

		va_end(args);

		D3DXGetImageInfoFromFile(path, &sprite->tImageInfo);
		HRESULT hr = D3DXCreateSprite(g_pDirect3DDevice, &sprite->pSprite);

		if (FAILED(hr))
		{
			GetLogManagerEx()->LogError("Can't create sprite!", false);
		}

		hr = D3DXCreateTextureFromFileEx(g_pDirect3DDevice, path, sprite->tImageInfo.Width, sprite->tImageInfo.Height,
			sprite->tImageInfo.MipLevels, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
			NULL, NULL, &sprite->pTexture);

		if (FAILED(hr))
		{
			GetLogManagerEx()->LogError("Can't load the \"%s\"!", false, path);
		}

		sprite->x = x;
		sprite->y = y;
		sprite->width = w != -1 ? w : sprite->tImageInfo.Width;
		sprite->height = h != -1 ? h : sprite->tImageInfo.Height;

		if (w == -2)
		{
			sprite->width = this->width;
		}
		if (h == -2)
		{
			sprite->height = this->height;
		}

		render_objects.push_back(sprite);

		return sprite;
	}
	break;
	default:
	{
		return nullptr;
	}
	break;
	}
}

inline void Render9::CreateWnd()
{
	hWnd = NULL;
	hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "WindowClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
		GetLogManagerEx()->LogError("Can't register window class!", true);

	hWnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		"WindowClass",
		"QEngine",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800,
		640,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hWnd == NULL)
		GetLogManagerEx()->LogError("Can't create window!", true);

	this->width = 800;
	this->height = 800;
}

inline void Render9::CreateDirectX()
{
	if ((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		GetLogManagerEx()->LogError("Can't create Direct3D!", true);

	D3DPRESENT_PARAMETERS PresParam;
	ZeroMemory(&PresParam, sizeof(PresParam));

	HRESULT hr = NULL;

	D3DDISPLAYMODE DisplayMode;
	hr = g_pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&DisplayMode);

	if (FAILED(hr))
		GetLogManagerEx()->LogError("Can't get display adapter!", true);

	PresParam.hDeviceWindow = hWnd;
	PresParam.Windowed = true;
	PresParam.BackBufferWidth = 800;
	PresParam.BackBufferHeight = 800;
	PresParam.BackBufferCount = 1;
	PresParam.EnableAutoDepthStencil = true;
	PresParam.AutoDepthStencilFormat = D3DFMT_D16;
	PresParam.SwapEffect = D3DSWAPEFFECT_FLIP;
	PresParam.BackBufferFormat = DisplayMode.Format;

	hr = g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&PresParam,
		&g_pDirect3DDevice);

	if (SUCCEEDED(hr))
		RenderType = 1;

	hr = g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&PresParam,
		&g_pDirect3DDevice);

	if (SUCCEEDED(hr))
		RenderType = 2;

	hr = g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&PresParam,
		&g_pDirect3DDevice);

	if (SUCCEEDED(hr))
		RenderType = 3;
}

inline void Render9::Shutdown()
{
	if(shutdownFunction)
		shutdownFunction();

	if (g_pDirect3DDevice != NULL)
	{
		g_pDirect3DDevice->Release();
		g_pDirect3DDevice = NULL;
	}

	if (g_pDirect3D != NULL)
	{
		g_pDirect3D->Release();
		g_pDirect3D = NULL;
	}

	if (!DestroyWindow(hWnd))
		hWnd = NULL;

	if (!UnregisterClass("WindowClass", hInstance))
		hInstance = NULL;
}

void Render9::RenderIn()
{
	if (preFunction)
		preFunction();

	HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST)
		return;

	g_pDirect3DDevice->Clear(
		0L,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f,
		0L);

	g_pDirect3DDevice->BeginScene();

	//RenderHere
	for (size_t i = 0; i < render_objects.size(); i++)
	{
		render_objects[i]->Render();
	}

	g_pDirect3DDevice->EndScene();
	g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	if(postFunction)
		postFunction();
}

Render9* render;

QRENDER9_PUBLIC RenderManager* CreateDirect9Render()
{
	render = new Render9();

	return dynamic_cast<RenderManager*>(render);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		render->NeedToClose = true;
		return 0;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}