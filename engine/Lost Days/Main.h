//Подключаем библиотеки
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dinput8.lib")

//Подключение стороннего кода
#include <objbase.h>
#include <initguid.h>
#include <dxgi.h>
#include <d3dx9math.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "CSpriteManager.h"

CSpriteManager CSM;

//Создаем переменные
HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;

int g_iWindowWidth = 800;
int g_iWindowHeight = 600;

bool g_bApplicationState = true;

bool ConsoleShow = false;

LPD3DXFONT pFont = NULL;
HFONT hFont;

RECT Rec;

IDirect3D9* g_pDirect3D = NULL;
IDirect3DDevice9* g_pDirect3DDevice = NULL;

#include "Input.h"

DxInput InputObj;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow); //Точка старта приложения
long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);//Обработчик сообщений
bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat);    //Инициализация Direct3D
void DrawFrame();                          //Рисуем кадр
void Shutdown();                          //Освобождаем память

void DrawTextD3D(LPDIRECT3DDEVICE9 pDirect3DDevice, HFONT hFont, char* Text, int x, int y, int x1, int y1, D3DCOLOR MyColor);