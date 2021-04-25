#include <Base_include.h>
#include "directx9.h"

#pragma warning( push )
#pragma warning( disable : 4005)
#include <d3dx9.h>
#pragma warning( pop ) 

DirectX9Manager* manager;

#include "../../Interfaces/IRenderable.h"
#include "IRenderable9.h"

bool InitDirectX9(Function PreUpdate)
{
	manager = new DirectX9Manager(800, 640, "QEngine", PreUpdate);

	if (manager)
		return true;
	else
		return false;

	return true;
}

DirectX9Manager* GetDX9Manager()
{
	return manager;
}

HRESULT DirectX9Manager::InitWnd(int w, int h, WNDPROC WndProc, const char* WndName)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "WindowClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
		return E_FAIL;

	rc = { 0, 0, w, h };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow("WindowClass", WndName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, g_hInst,
		NULL);

	ShowWindow(g_hWnd, true);

	return S_OK;
}

HRESULT DirectX9Manager::InitDevice()
{
	if ((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return S_FALSE;

	D3DPRESENT_PARAMETERS PresParam;
	ZeroMemory(&PresParam, sizeof(PresParam));

	HRESULT hr = NULL;

	D3DDISPLAYMODE DisplayMode;
	hr = g_pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&DisplayMode);

	if (FAILED(hr))
		return S_FALSE;

	PresParam.hDeviceWindow = g_hWnd;
	PresParam.Windowed = true;
	PresParam.BackBufferWidth = 800;
	PresParam.BackBufferHeight = 640;
	PresParam.BackBufferCount = 1;
	PresParam.EnableAutoDepthStencil = true;
	PresParam.AutoDepthStencilFormat = D3DFMT_D16;
	PresParam.SwapEffect = D3DSWAPEFFECT_FLIP;
	PresParam.BackBufferFormat = DisplayMode.Format;

	hr = g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&PresParam,
		&g_pDirect3DDevice);

	if (SUCCEEDED(hr))
		return S_FALSE;

	hr = g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&PresParam,
		&g_pDirect3DDevice);

	if (SUCCEEDED(hr))
		return S_FALSE;

	hr = g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&PresParam,
		&g_pDirect3DDevice);

	if (SUCCEEDED(hr))
		return S_FALSE;

	return S_OK;
}

LRESULT CALLBACK DirectX9Manager::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if (PreUpdateFunction)
		PreUpdateFunction(hWnd, iMsg, wParam, lParam);

	PAINTSTRUCT ps;
	HDC hdc;

	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}

	return 0;
}

DirectX9Manager::DirectX9Manager(float w, float h, const char* WndName, Function PreUpdateFunction)
{
	g_hInst = GetModuleHandle(NULL);
	g_hWnd = NULL;

	g_pDirect3D = NULL;
	g_pDirect3DDevice = NULL;

	HRESULT result = InitWnd((int)w, (int)h, (WNDPROC)WndProc, WndName);
	if (FAILED(result))
		GetLogManagerEx()->LogError("Can't create window.", true);

	result = InitDevice();
	if (FAILED(result))
		GetLogManagerEx()->LogError("Can't init DX11", true);

	this->PreUpdateFunction = PreUpdateFunction;

	count = 0;
	render[count] = nullptr;
}

Function DirectX9Manager::PreUpdateFunction;

DirectX9Manager::~DirectX9Manager()
{
	Shutdown();
}

void DirectX9Manager::Render()
{
	HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST)
		return;

	g_pDirect3DDevice->Clear(
		0L,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(0, 1, 1, 1),
		1.0f,
		0L);

	g_pDirect3DDevice->BeginScene();

	//Render
	for (int i = 0; i != count; i++)
	{
		render[i]->Render9->Render();
	}

	g_pDirect3DDevice->EndScene();

	g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

void DirectX9Manager::Update()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}
}

void DirectX9Manager::Shutdown()
{
	if (g_pDirect3DDevice)
		g_pDirect3DDevice->Release();

	if (g_pDirect3D)
		g_pDirect3D->Release();
}

IRenderable* DirectX9Manager::CreateIRenderable(const char* Sprite)
{
	IRenderable* render = new IRenderable();
#if IS_WIN10
	render->Render12 = nullptr;
#endif
	render->Render11 = nullptr;
	render->Render9 = new IRenderable9(g_pDirect3DDevice, Sprite);

	this->render[count++] = render;

	return render;
}