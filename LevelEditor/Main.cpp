#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

#pragma warning( disable : 26495 6031 4005 )

#include <objbase.h>
#include <initguid.h>
#include <dxgi.h>
#include <d3dx9.h>
#include <d3dtypes.h>
#include <d3dx9math.h>
#include <string.h>
#include <sstream>
#include <vector>

#include "Main.h"
#include "CSpriteManager.h"

#include "Utils.h"

#include "resource.h"

#include "Raw.h"

#include "Compiler.h"

HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;

HWND hWndMenu = NULL;

static HWND hBtnFlor;
static HWND hBtnDelete;

CLogManager LogManager;

int g_iWindowWidth = 800;
int g_iWindowHeight = 600;

IDirect3D9* g_pDirect3D = NULL;
IDirect3DDevice9* g_pDirect3DDevice = NULL;

FileSystem fs;

bool g_bApplicationState = true;

CSpriteManager SpriteManager;

char* CurrentCursorSprite = (char*)"";

float CurrentCursorSpritePosX = 0;

float CurrentCursorSpritePosY = 0;

char* CurrentFile = (char*)"";

bool CanDrawScene = true;

WORD xPos, yPos;

Compiler* CompilerObj = new Compiler();

long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat);
void CreateMenuUI();
void DrawFrame();
void Shutdown();

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;   // Позиция вершины
	DWORD color;        // Цвет вершины
};

#define D3DFVF_CUSTOMVERTEX D3DFVF_XYZRHW | D3DFVF_DIFFUSE

#define ID_HOTKEY_CTRL_Z 0x0CDC31337

int countOn1Scene = 0;
std::map<int, Vector2<CSprite*, Vector2<float, float>>> SceneSprites;

//std::map<Trigger, Vector2<float, float>> SceneTriggers;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w+", stdout);
	freopen("CONIN$", "w+", stdin);
#endif // _DEBUG

#ifdef _3D
	if (FindWord((char*)lpCmdLine, (char*)"-2D"))
	{
#endif // _3D

		std::string str = std::string(lpCmdLine);

		vector<std::string> tokens;

		for (auto i = strtok(&str[0], " "); i != NULL; i = strtok(NULL, " "))
		{
			if (FindWord(i, (char*)"-w"))
			{
				tokens.push_back(i);
				i = strtok(NULL, " ");
				g_iWindowWidth = atoi(i);
			}

			if (FindWord(i, (char*)"-h"))
			{
				tokens.push_back(i);
				i = strtok(NULL, " ");
				g_iWindowHeight = atoi(i);
			}
		}

		fs = FileSystem();

		LogManager.Init();

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
			LogManager.LogError((char*)"Can't register window class", true);
		}

		LogManager.LogMsg((char*)"Register window class");

		g_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			"WindowClass",
			"Level Editor",
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
			LogManager.LogError((char*)"Can't create window", true);
		}

		LogManager.LogMsg((char*)"Window created");

		if (!InitDirect3D(D3DFMT_R5G6B5, D3DFMT_D16))
		{
			LogManager.LogError((char*)"Can't get DirectX context", true);
		}

		LogManager.LogMsg((char*)"Getted DirectX context");

		ShowWindow(g_hWnd, SW_SHOW);
		UpdateWindow(g_hWnd);
		SetFocus(g_hWnd);
		SetForegroundWindow(g_hWnd);

#ifdef _3D
	}
#endif // _3D

	HMENU hMainMenu = CreateMenu();
	HMENU hPopMenuFile = CreatePopupMenu();

	AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuFile, "File");
	AppendMenu(hMainMenu, MF_STRING, 1000, "Help");

	AppendMenu(hPopMenuFile, MF_STRING, 1001, "Save");
	AppendMenu(hPopMenuFile, MF_STRING, 1002, "Save as...");
	AppendMenu(hPopMenuFile, MF_SEPARATOR, 1003, "");
	AppendMenu(hPopMenuFile, MF_STRING, 1004, "Load");
	AppendMenu(hPopMenuFile, MF_STRING, 1007, "Unload");
	AppendMenu(hPopMenuFile, MF_SEPARATOR, 1005, "");
	AppendMenu(hPopMenuFile, MF_STRING, 1008, "Compile!");
	AppendMenu(hPopMenuFile, MF_STRING, 1006, "Exit");

	SetMenu(g_hWnd, hMainMenu);
	SetMenu(g_hWnd, hPopMenuFile);

	//SetMenu(g_hWnd, hMenu);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	WNDCLASSEX wcMenu;
	wcMenu.cbSize = sizeof(WNDCLASSEX);
	wcMenu.style = CS_HREDRAW | CS_VREDRAW;
	wcMenu.lpfnWndProc = (WNDPROC)WndProc;
	wcMenu.cbClsExtra = 0;
	wcMenu.cbWndExtra = 0;
	wcMenu.hInstance = g_hInstance;
	wcMenu.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcMenu.hCursor = LoadCursor(0, IDC_ARROW);
	wcMenu.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcMenu.lpszMenuName = 0;
	wcMenu.lpszClassName = "WindowMenuClass";
	wcMenu.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcMenu))
	{
		LogManager.LogError((char*)"Can't register menu window class", true);
	}

	hWndMenu = CreateWindow(
		"WindowMenuClass",
		"Menu",
		WS_OVERLAPPEDWINDOW,
		860, 100,
		500, 800,
		NULL,
		NULL,
		g_hInstance,
		NULL
	);

	if (!hWndMenu)
	{
		LogManager.LogError((char*)"Can't create menu window", true);
		return 1;
	}

	CreateMenuUI();

	ShowWindow(hWndMenu, SW_SHOW);
	UpdateWindow(hWndMenu);
	SetFocus(hWndMenu);
	SetForegroundWindow(hWndMenu);

	SpriteManager.LoadAllSprites();

	if (RegisterHotKey(g_hWnd, ID_HOTKEY_CTRL_Z, MOD_CONTROL, 0x5A) == 0)
	{
		LogManager.LogError((char*)"Failed to create hot key", true);
	}

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

		if (!g_bApplicationState)
		{
			Shutdown();
		}
	}

	return 0;
}

void CreateMenuUI()
{
	//HBITMAP hBitmap = (HBITMAP)LoadImageA(g_hInstance, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
	HBITMAP hBitmap = (HBITMAP)LoadImageA(g_hInstance, (LPCSTR)"Flor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	hBtnFlor = CreateWindow((LPCSTR)"button", (LPCSTR)"flor", WS_VISIBLE | WS_CHILD | BS_BITMAP,
		50, 50, 80, 25, hWndMenu, NULL, NULL, NULL);
	SendMessage(hBtnFlor, BM_SETIMAGE, IMAGE_BITMAP, LPARAM(hBitmap));

	ShowWindow(hBtnFlor, SW_SHOWNORMAL);

	hBtnDelete = CreateWindow((LPCSTR)"button", (LPCSTR)"Delete", WS_VISIBLE | WS_CHILD /*| BS_BITMAP*/,
		200, 50, 80, 25, hWndMenu, NULL, NULL, NULL);

	ShowWindow(hBtnDelete, SW_SHOWNORMAL);
}

void RenderScene()
{
	if (CanDrawScene)
	{
		auto it = SceneSprites.begin();

		while (it != SceneSprites.end())
		{
			//Ебать вложенность
			float x = it->second.second.first;
			float y = it->second.second.second;

			float width = it->second.first->GetW();

			float height = it->second.first->GetH();

			width = width / GetWindowW() * 100;

			height = height / GetWindowH() * 100;

			it->second.first->Render(x, y, width, height, D3DCOLOR_XRGB(255, 255, 255));

			it++;
		}
	}
}

#pragma message("REWORK RAW LEVEL SAVE!")

void UnloadScene()
{
	SceneSprites.clear();

	SetWindowText(g_hWnd, "Level Editor");
}

void OpenRawLevel(char* RawLevelName)
{
	CanDrawScene = false;

	auto iss = std::istringstream((std::string)fs.ReadFromFile(RawLevelName));
	auto str = std::string{};

	while (iss >> str)
	{
		int id = 0;

		float x = 0.0f;

		float y = 0.0f;

		char* SpriteName = (char*)malloc(128);

		//ID
		char* ID = (char*)str.c_str();

		id = atoi(ID);

		iss >> str;

		char* X = (char*)str.c_str();

		x = (float)atoi(X);

		iss >> str;

		char* Y = (char*)str.c_str();

		y = (float)atoi(Y);

		iss >> str;

		strcpy(SpriteName, (char*)str.c_str());

		char* f = strtok(SpriteName, ".");

		SceneSprites[id] = { SpriteManager.GetSprite(f), {x, y} };

		countOn1Scene++;
	}

	CanDrawScene = true;
}

char* SaveLevelRaw()
{
	char* Buffer = (char*)malloc(1048576);

	strcpy(Buffer, "\n");

	auto it = SceneSprites.begin();

	while (it != SceneSprites.end())
	{
		int id = it->first;
		char* idB = (char*)malloc(32);
		_itoa(id, idB, 10);

		float x = it->second.second.first;
		char xB[10];
		_itoa(x, xB, 10);

		float y = it->second.second.second;
		char yB[10];
		_itoa(y, yB, 10);

		char* CSpriteName = it->second.first->GetName();

		strcat(Buffer, " ");
		strcat(Buffer, idB);
		strcat(Buffer, " ");

		strcat(Buffer, xB);
		strcat(Buffer, " ");

		strcat(Buffer, yB);
		strcat(Buffer, " ");

		strcat(Buffer, CSpriteName);
		strcat(Buffer, "\0");

		it++;
	}

	return Buffer;
}

long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	xPos = LOWORD(lParam);
	yPos = HIWORD(lParam);

	char* WindowsName = (char*)malloc(512);
	strcpy(WindowsName, "Level Editor: ");

	int iResults = 0;

	switch (iMsg)
	{
	case WM_DESTROY:
	{
		g_bApplicationState = false;
		return 0;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case 1000:
			MessageBox(g_hWnd, "Level editor v0.1A\nDeveloper: VAX", "Level Editor Help", MB_OK);
			break;
		case 1001:
			if (CurrentFile == "")
			{
				CurrentFile = SaveFileAs(g_hWnd, SaveLevelRaw());
				strcat(WindowsName, CurrentFile);
				SetWindowText(g_hWnd, WindowsName);
			}
			else
			{
				SaveFile(CurrentFile, (char*)SaveLevelRaw());
				strcat(WindowsName, CurrentFile);
				SetWindowText(g_hWnd, WindowsName);
			}
			break;
		case 1002:
			CurrentFile = SaveFileAs(g_hWnd, SaveLevelRaw());
			strcat(WindowsName, CurrentFile);
			SetWindowText(g_hWnd, WindowsName);
			break;
		case 1004:
			iResults = MessageBox(g_hWnd, "Are you sure wan't to load new level?\nAll unsaved data will errased! Procceed?", "Are you sure?", MB_YESNO);
			switch (iResults)
			{
			case IDYES:
				UnloadScene();
				CurrentFile = OpenRawFile(g_hWnd);
				OpenRawLevel(CurrentFile);
				strcat(WindowsName, CurrentFile);
				SetWindowText(g_hWnd, WindowsName);
				break;
			case IDNO:
				break;
			}
			break;
		case 1006:
			g_bApplicationState = 0;
			break;
		case 1007:
			iResults = MessageBox(g_hWnd, "Are you sure wan't to unload level?\nAll unsaved data will errased! Procceed?", "Are you sure?", MB_YESNO);
			switch (iResults)
			{
			case IDYES:
				UnloadScene();
				break;
			case IDNO:
				break;
			}
			break;
		case 1008:
			if(CurrentFile == "")
			{
				CurrentFile = SaveFileAs(g_hWnd, SaveLevelRaw());
				strcat(WindowsName, CurrentFile);
				SetWindowText(g_hWnd, WindowsName);
			}

			std::string str = std::string(CurrentFile);

			vector<std::string> tokens;

			BaseData bd;

			for (auto i = strtok(&str[0], "\\"); i != NULL; i = strtok(NULL, "\\"))
			{
				if (FindWord(i, (char*)".rawlvl"))
				{
					tokens.push_back(i);
					
					std::string str2 = std::string(i);

					auto j = strtok(&str2[0], ".");

					bd.LevelName = (char*)malloc(512);

					strcpy(bd.LevelName, j);
				}
			}

			Physic physic;
			physic.gravity = 10;

			bd.PhysicData = physic;

			CompilerObj->WriteBaseData(&bd);

			break;
		}

		if (lParam == (LPARAM)hBtnFlor && hWnd == hWndMenu)    // если нажали на кнопку
		{
			SpriteManager.SetSpriteVisible(true, (char*)"Flor");
			CurrentCursorSprite = (char*)"Flor";
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		printf("Sprite num:%d | x: %i | y: %i | \n", countOn1Scene + 1, (int)CurrentCursorSpritePosX, (int)CurrentCursorSpritePosY);
		if (GetFocus() == g_hWnd && CurrentCursorSprite != "")
		{
			Vector2<CSprite*, Vector2<float, float>> sprite = { SpriteManager.GetSprite(CurrentCursorSprite), {CurrentCursorSpritePosX, CurrentCursorSpritePosY} };
			SceneSprites[countOn1Scene] = sprite;
			countOn1Scene++;
			//SceneTriggers[]
		}
	}
	case WM_HOTKEY:
	{
		if (wParam == 3452113719)
		{
			if (countOn1Scene > 0)
			{
				SceneSprites.erase(countOn1Scene - 1);
				printf("Deleted sprite on scene1 - id: %d \n", countOn1Scene - 1);
				countOn1Scene--;
			}
		}
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

//void DrawLine(LPDIRECT3DDEVICE9 Device_Interface, float x1, float x2, float y1, float y2, D3DCOLOR COLOR)
//{
//	CUSTOMVERTEX vertices[] =
//	{
//		//{ 50.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, },// x, y, z, rhw, цвет
//		//{ 250.0f, 250.0f, 0.5f, 1.0f, 0xffff0000, },
//		//{  50.0f, 250.0f, 0.5f, 1.0f, 0xffff0000, },
//		{ x1, x1, 0.5f, 1.0f, COLOR, },// x, y, z, rhw, цвет
//		{ y1, y1, 0.5f, 1.0f, COLOR, },
//		{ x1, y2, 0.5f, 1.0f, COLOR, },
//	};
//
//	CUSTOMVERTEX vertices2[] =
//	{
//		//{ 50.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, },// x, y, z, rhw, цвет
//		//{ 250.0f, 50.0f, 0.5f, 1.0f, 0xffff0000, },
//		//{  250.0f, 250.0f, 0.5f, 1.0f, 0xffff0000, },
//		{ x1, x1, 0.5f, 1.0f, COLOR, },// x, y, z, rhw, цвет
//		{ x2, x1, 0.5f, 1.0f, COLOR, },
//		{ y2, y2, 0.5f, 1.0f, COLOR, },
//	};
//
//	IDirect3DVertexBuffer9* g_pVB;
//
//	if (FAILED(Device_Interface->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
//	{
//		LogManager.LogError((char*)"Can't create vertex buffer!", true);
//	}
//
//	VOID* pVertices;
//
//	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
//	{
//		LogManager.LogError((char*)"Can't lock vertex buffer!", true);
//	}
//
//	memcpy(pVertices, vertices, sizeof(vertices));
//
//	g_pVB->Unlock();
//
//	Device_Interface->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
//
//	Device_Interface->SetFVF(D3DFVF_CUSTOMVERTEX);
//
//	Device_Interface->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
//
//	//---------------------------------------------------------------------------
//
//	VOID* pVertices2;
//
//	if (FAILED(g_pVB->Lock(0, sizeof(vertices2), (void**)&pVertices2, 0)))
//	{
//		LogManager.LogError((char*)"Can't lock vertex buffer!", true);
//	}
//
//	memcpy(pVertices2, vertices2, sizeof(vertices2));
//
//	g_pVB->Unlock();
//
//	Device_Interface->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
//
//	Device_Interface->SetFVF(D3DFVF_CUSTOMVERTEX);
//
//	Device_Interface->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
//}

//bw - длинна
void DrawLineGorizontal(LPDIRECT3DDEVICE9 Device_Interface, int bx, int by, int bw, int thick, D3DCOLOR COLOR)
{
	D3DRECT rec;
	rec.x1 = bx;//makes line longer/shorter going lef
	rec.y1 = by; // base y
	rec.x2 = bx + bw; //makes line longer/shorter going right
	rec.y2 = by + thick; //makes line one pixel tall

	Device_Interface->Clear(1, &rec, D3DCLEAR_TARGET, COLOR, 0, 0);
}

//bw - длинна
void DrawLineVertical(LPDIRECT3DDEVICE9 Device_Interface, int bx, int by, int bw, int thick, D3DCOLOR COLOR)
{
	D3DRECT rec;
	rec.x1 = bx;
	rec.y1 = by;
	rec.x2 = bx + thick;
	rec.y2 = by + bw;

	Device_Interface->Clear(1, &rec, D3DCLEAR_TARGET, COLOR, 0, 0);
}

void DrawLevelEditorMesh()
{
	int y = 0;
	int i = 0;

	while (y < g_iWindowHeight / 1.3)
	{
		DrawLineGorizontal(g_pDirect3DDevice, 60, y + 60, 720, 5, D3DCOLOR_XRGB(127, 127, 127));

		y = y + 30;
	}

	while (i < g_iWindowWidth / 1.8)
	{
		DrawLineVertical(g_pDirect3DDevice, i + 60, 60, 545, 5, D3DCOLOR_XRGB(127, 127, 127));

		i = i + 30;
	}
}

//Это останеться для будущего меня чисто чтоб поржать
//void DrawLevelEditorMesh()
//{
//	float widthW = g_iWindowWidth / 225;
//	float heightW = g_iWindowHeight / 225;
//
//	if (!LineInit)
//	{
//		spriteLine = new CSprite(g_pDirect3DDevice, (LPCSTR)"../gamedata/sprites/LevelEditor/Line.png", g_iWindowWidth, g_iWindowHeight);
//
//		printf("widthW is %f \n", widthW);
//		printf("heightH is %f \n", heightW);
//	}
//
//	float width = spriteLine->GetW();
//
//	width = width / GetWindowW() * 100;
//
//	if(!LineInit)
//	{
//		printf("width is %f \n", width);
//	}
//
//	int y = 0;
//	int i = 0;
//
//	float LenghtOfSpriteW = heightW * 41.83;
//	float LenghtOfSpriteH = heightW * 26;
//
//	if (!LineInit)
//	{
//		printf("LenghtOfSpriteW is %f \n", LenghtOfSpriteW);
//		printf("LenghtOfSpriteH is %f \n", LenghtOfSpriteH);
//	}
//
//	while(y < heightW*202)
//	{
//		spriteLine->Render(60, y+60, width - 0.8, LenghtOfSpriteW, D3DCOLOR_ARGB(128, 255, 255, 255), -90);
//
//		while (i < widthW*161)
//		{
//			//Волшебство: альфа канал работает через залупу
//			//Поправочка - работал* через залупу
//			spriteLine->Render(i+60, 60, width - 0.8, LenghtOfSpriteH, D3DCOLOR_ARGB(128, 255, 255, 255), 0);
//			i = i + 15;
//		}
//		y = y + 15;
//	}
//
//	if (!LineInit)
//	{
//		printf("i is %d \n", i);
//		printf("y is %d \n", y);
//		LineInit = true;
//	}
//}

void DrawFrame()
{
	HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST)
		return;

	g_pDirect3DDevice->Clear(
		0L,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 255, 255),
		1.0f,
		0L);

	g_pDirect3DDevice->BeginScene();

	if (xPos >= 60 && xPos <= 760 + 60
		&&
		yPos >= 60 && yPos <= 550 + 60)
	{
		if (CurrentCursorSprite != "")
		{
			float x = xPos / 10;
			float y = yPos / 10;

			x = floor(x);
			y = floor(y);

			x = x * 10;
			y = y * 10;

			CurrentCursorSpritePosX = x /*+ SpriteManager.GetSprite(CurrentCursorSprite)->GetW()/2*/;

			CurrentCursorSpritePosY = y /*+ SpriteManager.GetSprite(CurrentCursorSprite)->GetH() / 2*/;

			SpriteManager.MoveSprite(CurrentCursorSprite, x, y);
		}
	}
	else
	{
		//SpriteManager.MoveSprite(CurrentCursorSprite, 60, 60);
	}

	RenderScene();

	SpriteManager.RenderAllSprites();

	DrawLevelEditorMesh();

	g_pDirect3DDevice->EndScene();
	g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

void Shutdown()
{
	if (UnregisterHotKey(g_hWnd, ID_HOTKEY_CTRL_Z) == 0)
	{
		LogManager.LogError((char*)"Unregister hot key failed!", false);
	}

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

	if (!DestroyWindow(g_hWnd))
		g_hWnd = NULL;

	if (!UnregisterClass("WindowClass", g_hInstance))
		g_hInstance = NULL;
}

IDirect3DDevice9* GetD3D9Device()
{
	return g_pDirect3DDevice;
}

int GetWindowH()
{
	return g_iWindowHeight;
}

int GetWindowW()
{
	return g_iWindowWidth;
}

CLogManager* GetLogObjCl()
{
	return &LogManager;
}

FileSystem* GetFileSystemObjCl()
{
	return &fs;
}