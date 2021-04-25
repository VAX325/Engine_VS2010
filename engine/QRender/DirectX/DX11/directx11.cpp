#include <Base_include.h>
#include "directx11.h"

#pragma warning( push )
#pragma warning( disable : 4005)
#include <d3d11.h>
#pragma warning( pop ) 

DirectX11Manager* manager;

#include "../../Interfaces/IRenderable.h"
#include "IRenderable11.h"

bool InitDirectX11(Function PreUpdate)
{
	manager = new DirectX11Manager(800, 640, "QEngine", PreUpdate);

	if (manager)
		return true;
	else
		return false;
}

DirectX11Manager* GetDX11Manager()
{
	return manager;
}

HRESULT DirectX11Manager::InitWnd(int w, int h, WNDPROC WndProc, const char* WndName)
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

HRESULT DirectX11Manager::InitDevice()
{
	HRESULT result;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		result = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(result))
			break;
	}
	if (FAILED(result))
		return result;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	result = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(result))
		return result;

	result = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(result))
		return result;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

	//установка Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = static_cast<float>(sd.BufferDesc.Width);
	viewport.Height = static_cast<float>(sd.BufferDesc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &viewport);

	return S_OK;
}

LRESULT CALLBACK DirectX11Manager::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if(PreUpdateFunction)
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

DirectX11Manager::DirectX11Manager(float w, float h, const char* WndName, Function PreUpdateFunction)
{
	g_hInst = GetModuleHandle(NULL);
	g_hWnd = NULL;

	g_driverType = D3D_DRIVER_TYPE_NULL;
	g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	g_pd3dDevice = NULL;
	g_pImmediateContext = NULL;
	g_pSwapChain = NULL;
	g_pRenderTargetView = NULL;
	
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

Function DirectX11Manager::PreUpdateFunction;

DirectX11Manager::~DirectX11Manager()
{
	Shutdown();
}

void DirectX11Manager::Render()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	//g_pImmediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	g_pImmediateContext;

	for (int i = 0; i != count; i++) 
	{
		render[i]->Render11->Render(g_pImmediateContext);
	}

	g_pImmediateContext->DrawAuto();

	g_pSwapChain->Present(0, 0);
}

void DirectX11Manager::Update()
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

void DirectX11Manager::Shutdown()
{
	if (g_pImmediateContext)
		g_pImmediateContext->ClearState();

	if (g_pRenderTargetView)
		g_pRenderTargetView->Release();

	if (g_pSwapChain)
		g_pSwapChain->Release();

	if (g_pImmediateContext)
		g_pImmediateContext->Release();

	if (g_pd3dDevice)
		g_pd3dDevice->Release();
}

IRenderable* DirectX11Manager::CreateIRenderable(const char* Sprite)
{
	IRenderable* render = new IRenderable();
#if IS_WIN10
	render->Render12 = nullptr;
#endif
	render->Render11 = new IRenderable11(this->g_pd3dDevice, Sprite, this->g_pImmediateContext);
	render->Render9 = nullptr;

	this->render[count++] = render;

	return render;
}
