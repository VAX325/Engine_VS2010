#pragma once
#ifndef DIRECTX9_H
#define DIRECTX9_H

#include <Windows.h>
#pragma warning( push )
#pragma warning( disable : 4005)
#include <d3d9.h>
#pragma warning( pop ) 

#include "../../BaseManager.h"
#include "../../Interfaces/IRenderable.h"

typedef void (*Function)(HWND, UINT, WPARAM, LPARAM);

class DirectX9Manager : public GraphicsManager
{
public:
	DirectX9Manager(float w, float h, const char* WndName, Function PreUpdateFunction);
	~DirectX9Manager();

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

	//D3D9
	IDirect3D9* g_pDirect3D;
	IDirect3DDevice9* g_pDirect3DDevice;

	RECT rc;

	static Function PreUpdateFunction;
};

bool InitDirectX9(Function PreUpdate);
DirectX9Manager* GetDX9Manager();

#endif