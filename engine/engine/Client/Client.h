#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dinput8.lib")

#pragma warning( disable : 26495 6031 4005 )

#include "ClientPerems.h"

#include <objbase.h>
#include <initguid.h>
#include <dxgi.h>
#include <d3dx9math.h>
#include <iostream>
#include <vector>
#include <fstream>

int ClientMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow);
long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat);
void DrawFrame();
void Shutdown();

float LuaGetFPS();