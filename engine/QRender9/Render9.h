#pragma once
#ifndef RENDER9_H
#define RENDER9_H

class RenderManager;
enum class RenderableType;

struct IDirect3D9;
struct IDirect3DDevice9;

class Render9 : public RenderManager
{
public:
	Render9();
	virtual ~Render9();

	typedef void(*PreRenderFunction)();
	typedef void(*RenderFunction)();
	typedef void(*PostRenderFunction)();

	virtual void SetPreRender(PreRenderFunction pre_function);
	virtual void SetRender(RenderFunction render_function);
	virtual void SetPostRender(PostRenderFunction post_function);

	virtual void Render();

	virtual IRenderable* CreateRenderable(RenderableType type, ...);

private:
	friend LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);;

	inline void CreateWnd();
	inline void CreateDirectX();
	inline void Shutdown();

	int RenderType;

	void RenderIn();

	int width, height;

	//Wnd
	HWND hWnd;
	HINSTANCE hInstance;
	bool NeedToClose;

	//DirectX
	IDirect3D9* g_pDirect3D;
	IDirect3DDevice9* g_pDirect3DDevice;
};

QRENDER9_PUBLIC RenderManager* CreateDirect9Render();

#endif