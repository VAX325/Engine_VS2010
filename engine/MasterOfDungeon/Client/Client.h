#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dinput8.lib")

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

void DrawTextD3D(LPDIRECT3DDEVICE9 pDirect3DDevice, HFONT hFont, char* Text, int x, int y, int x1, int y1, D3DCOLOR MyColor);