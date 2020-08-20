#include "Main.h"
#include "CLogManager.h"

CLogManager LogManager = CLogManager();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	g_hWnd = NULL;
	g_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "WindowClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		LogManager.LogError("Can't register window class", true);
	}

	LogManager.LogMsg("Register window class");

	g_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		"WindowClass",
		"Master of Dungeon",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0,
		0,
		g_iWindowWidth,
		g_iWindowHeight,
		NULL,
		NULL,
		g_hInstance,
		NULL);

	if (g_hWnd == NULL)
	{
		LogManager.LogError("Can't create window", true);
	}
	
	LogManager.LogMsg("Window created");

	if (!InitDirect3D(D3DFMT_R5G6B5, D3DFMT_D16))
	{
		LogManager.LogError("Can't get DirectX context", true);
	}

	LogManager.LogMsg("Getted DirectX context");

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
	SetFocus(g_hWnd);
	SetForegroundWindow(g_hWnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	InputObj = DxInput();

	if (!InputObj.Initialize(g_hInstance, g_hWnd))
	{
		LogManager.LogError("Can't get DirectInput context", true);
	}

	LogManager.LogMsg("Getted DirectInput context");

	CSM = CSpriteManager();

	CSM.LoadAllSprites();

	while (g_bApplicationState)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DrawFrame();
		}
	}

	Shutdown();
	return 0;
}

long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
	{
		g_bApplicationState = false;
		return 0;
	}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);  //Если нету для нас нужных сообщений, пусть это обрабатывает виндовс
}

bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat)
{
	if ((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	D3DPRESENT_PARAMETERS PresParam;
	ZeroMemory(&PresParam, sizeof(PresParam));

	HRESULT hr = NULL;

	D3DDISPLAYMODE DisplayMode;
	hr = g_pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&DisplayMode);

	if (FAILED(hr))
		return false;

	PresParam.hDeviceWindow = g_hWnd;
	PresParam.Windowed = true;
	PresParam.BackBufferWidth = g_iWindowWidth;
	PresParam.BackBufferHeight = g_iWindowHeight;
	PresParam.BackBufferCount = 1;
	PresParam.EnableAutoDepthStencil = true;
	PresParam.AutoDepthStencilFormat = DepthFormat;
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
		return true;

	hr = g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&PresParam,
		&g_pDirect3DDevice);

	if (SUCCEEDED(hr))
		return true;

	hr = g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&PresParam,
		&g_pDirect3DDevice);

	if (SUCCEEDED(hr))
		return true;

	return false;
}

void DrawFrame()
{
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

	if (InputObj.Frame())
	{
		if (InputObj.IsKeyPressed(DIK_ESCAPE, 0x80))
		{
			g_bApplicationState = false;
		}
	}
	else
	{
		g_bApplicationState = false;
	}

	g_pDirect3DDevice->BeginScene();

	CSM.RenderAllSprites();

	g_pDirect3DDevice->EndScene();
	g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

void Shutdown()
{
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

	InputObj.ShutdownInput();

	if (!DestroyWindow(g_hWnd))
		g_hWnd = NULL;

	if (!UnregisterClass("WindowClass", g_hInstance))
		g_hInstance = NULL;
}

void DrawTextD3D(LPDIRECT3DDEVICE9 pDirect3DDevice, HFONT hFont, char* Text, int x, int y, int x1, int y1, D3DCOLOR MyColor)
{
	Rec.left = x;
	Rec.top = y;
	Rec.right = x1;
	Rec.bottom = y1;
	
	D3DXCreateFont(pDirect3DDevice, 30, 10, 1, 0, FALSE, 0, 30, 0, 0, "Arial", &pFont);
	
	pFont->DrawText(0, Text, -1, &Rec, NULL, MyColor);
}

IDirect3DDevice9* GetD3D9Device()
{
	return g_pDirect3DDevice;
}

HWND GetMainWnd()
{
	return g_hWnd;
}

int GetWindowH()
{
	return g_iWindowHeight;
}

int GetWindowW()
{
	return g_iWindowWidth;
}

CLogManager GetLogObj()
{
	return LogManager;
}