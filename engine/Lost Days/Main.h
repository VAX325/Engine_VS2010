//Подключаем библиотеки
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "assimp.lib")

//Подключение стороннего кода
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <objbase.h>
#include <initguid.h>
#include <dxgi.h>
#include <D3Dcommon.h>
#include <d3dx9math.h>
#include <dinput.h>
#include <assimp\Importer.hpp>
#include <assimp\mesh.h>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <iostream>
#include <vector>
#include <fstream>

//Определяем переменные
#define DIRECTINPUT_VERSION 0x0800
#define MAX_VERTICES 8000
#define MAX_POLYGONS 8000

//Создаем переменные
HINSTANCE g_hInstance = NULL;      //Дескриптор приложения
HWND g_hWnd = NULL;            //Дескриптор окна
int g_iWindowWidth = 800;        //Ширина окна
int g_iWindowHeight = 600;        //Высота окна
bool g_bApplicationState = true;    //Состояние приложения (true - работает/false - не работает)
IDirect3D9 *g_pDirect3D = NULL;      //Интерфейс для создания устройства рендеринга
IDirect3DDevice9 *g_pDirect3DDevice = NULL;  //Интерфейс устройства рендеринга
bool ConsoleShow = false;
LPD3DXFONT pFont = NULL;// Шрифт Direct3D
RECT Rec; //область размещения текста - Прямоугольник
HFONT hFont; // Шрифт
int ConsStrNumb = -10;
int Number = 0;


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow); //Точка старта приложения
long WINAPI WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);//Обработчик сообщений
bool InitDirect3D(D3DFORMAT ColorFormat,D3DFORMAT DepthFormat);    //Инициализация Direct3D
void DrawFrame();                          //Рисуем кадр
void Shutdown();                          //Освобождаем память

void DrawTextD3D(LPDIRECT3DDEVICE9 pDirect3DDevice,HFONT hFont, char* Text,int x,int y,int x1,int y1,D3DCOLOR MyColor);

void ReturnToBackLayer();

void LoadAllSprites();//Подгружает все спрайты из папки gamedata/sprites

void ExportToConsole(char* Str,int* Type);

class DxInput
{
    public:
        DxInput();
        ~DxInput();
 
        bool Initialize(HINSTANCE hinstance, HWND hwnd);
        void ShutdownInput();
        bool Frame();
 
        bool IsKeyPressed(int DIK,int DIK_NUMBER);     
 
        bool IsRMouseButtonPressed();
        bool IsLMouseButtonPressed();
 
        void MouseMove(int *deltaX, int *deltaY);
 
    private:
        bool ReadKeyboard();
        bool ReadMouse();   
 
    private:
        IDirectInput8* m_directInput;
        IDirectInputDevice8* m_keyboard;
        IDirectInputDevice8* m_mouse;
        unsigned char m_keyboardState[256];
        DIMOUSESTATE m_mouseState;  
};

class CSprite{
private:
    LPD3DXSPRITE pSprite; // sprite то с помощю чего мы будем рисовать
    LPDIRECT3DTEXTURE9 pTexture; // сама текстура
    D3DXIMAGE_INFO tImageInfo; // некоторая информация о картинке
    UINT WW,WH; // реальные размеры окна
public:
    CSprite(LPDIRECT3DDEVICE9 pD3DDevice,LPCSTR Path,UINT WindowWidth,UINT WindowHeight); // d3d device, путь к файлу, реальные размеры окна
    ~CSprite(void); // удаляем наш sprite
    void Render(float X,float Y,float Width,float Height,D3DCOLOR Color); // параметры float указывать в процентах 0 - 100
};

class Model
{
private:
	const aiScene* pScene, *scene;
	const aiMesh* pMesh;
public:
	bool LoadModel(const char* fileName);
	void InitMesh(const aiMesh* pMesh);
};

class Mesh
{
public:
	bool LoadMesh(const std::string& Filename);
};
