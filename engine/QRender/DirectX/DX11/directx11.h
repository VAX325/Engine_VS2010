#pragma once
#ifndef DIRECTX11_H
#define DIRECTX11_H

#include <Windows.h>
#pragma warning( push )
#pragma warning( disable : 4005)
#include <D3DX11.h>
#pragma warning( pop ) 

#include "../../BaseManager.h"
#include "../../Interfaces/IRenderable.h"

typedef void (*Function)(HWND, UINT, WPARAM, LPARAM);

class DirectX11Manager : public GraphicsManager
{
public:
	DirectX11Manager(float w, float h, const char* WndName, Function PreUpdateFunction);
	~DirectX11Manager();

	virtual void Update();
	virtual void Shutdown();

	virtual IRenderable* CreateIRenderable(const char* Sprite);
private:
	inline HRESULT InitWnd(int w, int h, WNDPROC WndProc, const char* WndName);
	inline HRESULT InitDevice();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Render();

	//Wnd
	HINSTANCE g_hInst;
	HWND g_hWnd;

	//D3D11
	D3D_DRIVER_TYPE g_driverType;
	D3D_FEATURE_LEVEL g_featureLevel;
	ID3D11Device* g_pd3dDevice;
	ID3D11DeviceContext* g_pImmediateContext;
	IDXGISwapChain* g_pSwapChain;
	ID3D11RenderTargetView* g_pRenderTargetView;

	RECT rc;

	static Function PreUpdateFunction;
};

bool InitDirectX11(Function PreUpdate);
DirectX11Manager* GetDX11Manager();

#endif