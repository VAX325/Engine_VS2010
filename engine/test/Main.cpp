#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#define TODO __FILE__ "(" $Line ") : TODO: "

//Use like: #if IS_WIN32
#define IS_WIN32 PLATFORM == PLATFORM_WINDOWS

//Use like: #if IS_MAC
#define IS_MAC PLATFORM == PLATFORM_MAC

//Use like: #if IS_UNIX
#define IS_UNIX PLATFORM == PLATFORM_UNIX

#include "../UsefullMacro.h"

#include <Windows.h>
#include "../QRender/Interfaces/QRender.h"

#pragma comment(lib, "QRender.lib")

#include <Windows.h>
#include <iostream>

void PreUpdate(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
}

int main()
{
	PrepareRenderEngine(Specifications::D3DX9, PreUpdate);

	IRenderable* render = GetGraphicsManager()->CreateIRenderable("Sprite.png");

	render->SetX(100);
	render->SetY(100);

	Update();

	ShutdownRenderEngine();
}