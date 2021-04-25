#include <Base_include.h>

#include "Client.h"
#include "CUIManager.h"
#include "CGameCL.h"
#include "../Server/Server.h"
#include "../xmlparser.h"
#include "../Utils.h"

#include <iostream>
#include <ctime>

HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;

int g_iWindowWidth = 800;
int g_iWindowHeight = 600;

bool g_bApplicationState = true;

HFONT hFont;

IDirect3D9* g_pDirect3D = NULL;
IDirect3DDevice9* g_pDirect3DDevice = NULL;

#include "Input.h"

CInput InputObj;

CSpriteManager CSM;

CSoundManager SoundManager;

CScriptSystem ScriptSystem;

CUIManager UIManager;

XMLParser* xml_parser;

lua_State* L;

CGameCL* Game;

struct Mouse
{
	int xPos;
	int yPos;
};

Mouse MouseCl;

void inline UpdateFuncUP(int button)
{
	UIManager.GetPanel(UIManager.GetCurrentPanel())->CheckForUIEvents(MouseCl.xPos, MouseCl.yPos);
}

class MouseReciverClass : public iMouseReceiver
{
public:
	void MouseButtonUp(int button) override
	{
		UpdateFuncUP(button);
	};

	void MouseButtonDown(int button) override
	{
		
	};
};

MouseReciverClass* MouseReciver = new MouseReciverClass();

bool Sound = false;

float fps = 0.0f;
static DWORD total = 0;
static DWORD frames = 0;

float LuaGetFPS()
{
	return fps;
}

void LuaCheckForEvents()
{
	
}

bool ClLaunched = false;
bool SvLaunched = false;

void LuaLevelChange(lua_State* LS)
{
	if (!ClLaunched && !SvLaunched)
	{
		ClLaunched = true;
		SvLaunched = true;

		SteamGameSocketsInit();

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ServerInClient, NULL, 0, NULL);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (lua_isstring(L, 1))
		{
			UIManager.HidePanel("MainMenu.xml");
			UIManager.ShowPanel("LoadingScreen.xml");

			char* LevelName = (char*)lua_tostring(L, 1);
			Game = new CGameCL(LevelName);

			while(Game->IsLevelLoading())
			{
				//Here need tips	
			}

			UIManager.HidePanel("LoadingScreen.xml");
			UIManager.ShowPanel("Game.xml");
		}
		else
		{
			GetLogManagerEx()->LogError("You atempting to load unknown level!", false);
		}
	}
	else
	{
		if(ClLaunched)
			Game->UnloadLevel();

		if (SvLaunched)
			ServerStop();

		ClLaunched = false;
		SvLaunched = false;
	}
}

int ClientMain(int argc, char* argv[])
{
#ifndef _DEBUG
	FreeConsole();
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#else
	if (::IsWindowVisible(::GetConsoleWindow()) == FALSE)
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
#endif

	InitLogManager(false);

	for (int i = 0; i != argc; i++)
	{
		if (FindWord((char*)argv[i], (char*)"-nosound"))
		{
			Sound = true;
			GetLogManagerEx()->LogMsg("No sound!");
		}
	}

	g_hWnd = NULL;
	g_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
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
		GetLogManagerEx()->LogError("Can't register window class", true);
	}

	GetLogManagerEx()->LogMsg("Register window class");

	g_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		"WindowClass",
		"Master of Dungeon",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_iWindowWidth,
		g_iWindowHeight,
		NULL,
		NULL,
		g_hInstance,
		NULL);

	if (g_hWnd == NULL)
	{
		GetLogManagerEx()->LogError("Can't create window", true);
	}

	GetLogManagerEx()->LogMsg("Window created");

	if (!InitDirect3D(D3DFMT_R5G6B5, D3DFMT_D16))
	{
		GetLogManagerEx()->LogError("Can't get DirectX context", true);
	}

	GetLogManagerEx()->LogMsg("Getted DirectX context");

	try
	{
		ShowWindow(g_hWnd, SW_SHOW);
		UpdateWindow(g_hWnd);
		SetFocus(g_hWnd);
		SetForegroundWindow(g_hWnd);
	}
	catch (const std::exception& e)
	{
		GetLogManagerEx()->LogError(e.what(), true);
	}

	InputObj = CInput();

	if (!InputObj.Initialize(g_hInstance, g_hWnd))
	{
		GetLogManagerEx()->LogError("Can't get DirectInput context", true);
	}

	InputObj.GetMouse()->SetReceiver(MouseReciver);

	GetLogManagerEx()->LogMsg("Getted DirectInput context");

	CSM = CSpriteManager();

	CSM.LoadAllSprites();

	if (!Sound)
	{
		SoundManager = CSoundManager();

		SoundManager.PlayBuffSound(0, 1.0f, true, 0.5, 1.0);
	}

	L = luaL_newstate();
	luaL_openlibs(L);

	ScriptSystem = CScriptSystem();

	InitLuaShared(L, GetLogManagerEx());

	luabridge::getGlobalNamespace(L)
		.addFunction("CheckForEvents", LuaCheckForEvents)
		.addFunction("LevelChange", LuaLevelChange);

	ScriptSystem.LuaStart(L);

	UIManager = CUIManager();

	UIManager.LoadPanels();

	UIManager.ShowPanel("MainMenu.xml");

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (g_bApplicationState)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD start = GetTickCount();

			DrawFrame();

			DWORD end = GetTickCount();

			total += end - start;

			frames++;

			if (total >= 1000) {
				fps = frames * 1000 / (float)total;

#ifdef _DEBUG
				//cout << "FPS: " << fps << "\n";
#endif
				frames = 0;
				total = 0;
			}
		}

		ScriptSystem.LuaUpdateCl(L);
		
		if(!g_bApplicationState)
		{
			Shutdown();
		}
	}

	return 0;
}

long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MouseCl.xPos = LOWORD(lParam);
	MouseCl.yPos = HIWORD(lParam);

	switch (iMsg)
	{
	case WM_DESTROY:
	{
		g_bApplicationState = false;
		return 0;
	}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
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
	
	if(!Sound)
	{
		SoundManager.Update();
	}
	
	//D3DXCreateFont(g_pDirect3DDevice, 30, 10, 1, 0, FALSE, 0, 30, 0, 0, "Arial", &pFont);
	//DrawTextD3D(g_pDirect3DDevice, pFont, FPSC, 0, 0, 1000, 700, D3DCOLOR_ARGB(250, 100, 100, 100));

	UIManager.RenderPanels();

	g_pDirect3DDevice->EndScene();

	g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

void Shutdown()
{
	Game->UnloadLevel();

	if (SvLaunched)
		ServerStop();

	SteamGameSocketsDeInit();

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

CInput* GetInputObj()
{
	return &InputObj;
}

CSpriteManager* GetSpriteManger()
{
	return &CSM;
}

CSoundManager* GetSoundObj()
{
	return &SoundManager;
}

CScriptSystem* GetScriptSystemObjCl()
{
	return &ScriptSystem;
}

lua_State* GetLuaStateCl()
{
	return L;
}

void CallLuaFuncCl(char* LuaFunc)
{
	ScriptSystem.CallLuaFunc(L, LuaFunc);
}

LuaFuncPtr* GetLuaFuncPtrCl(char* LuaName)
{
	LuaFuncPtr* _ptr = ScriptSystem.GetLuaFuncPtr(L, LuaName);

	return _ptr;
}