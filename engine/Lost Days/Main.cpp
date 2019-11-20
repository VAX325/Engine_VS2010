#include "Main.h"
DxInput InputObj;
char* CurrentLayer = "MainMenu";
char* BackLayer = "MainMenu";

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow)
{  
  g_hInstance = GetModuleHandle(NULL);

  WNDCLASSEX wc;
  wc.cbSize      = sizeof(WNDCLASSEX);        //Размер структуры
  wc.style           = CS_HREDRAW|CS_VREDRAW;      //Стили класса окна
  wc.lpfnWndProc    = WndProc;              //Функция обработки сообщений
  wc.cbClsExtra    = 0;                //Количество выделяемой памяти при создании приложения
  wc.cbWndExtra      = 0;                //Количество выделяемой памяти при создании приложения
  wc.hInstance    = g_hInstance;            //Дескриптор приложения
  wc.hIcon           = LoadIcon(NULL,IDI_APPLICATION);  //Загружаем стандартную иконку
  wc.hCursor         = LoadCursor(0,IDC_ARROW);      //Загружаем стандартный курсор
  wc.hbrBackground   = (HBRUSH)GetStockObject(WHITE_BRUSH);//Окно будет закрашено в белый цвет
  wc.lpszMenuName    = 0;                //Не используем меню
  wc.lpszClassName   = "WindowClass";            //Названия класса
  wc.hIconSm       = LoadIcon(NULL,IDI_APPLICATION);  //Загружаем стандартную иконку

  if(!RegisterClassEx(&wc))                //Регистрируем класс в Windows
  {
    Shutdown();                    //Освобождаем память
    MessageBox(NULL,"Не возможно зарегестрировать класс (WindowClass).\n За доп. информацией напишите разработчику.","Error",MB_OK|MB_ICONERROR); //Выводим сообщение
    return 0;                    //Завершаем работу приложения
  }

  g_hWnd = CreateWindowEx(              //Создаем окно
    WS_EX_APPWINDOW|WS_EX_WINDOWEDGE,        //Расширенный стиль окна
    "WindowClass",                    //Названия класса окна
    "Lost Days",    //Названия окна
    WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,//Стиль окна
    0,                      //Позиция окна по оси Х
    0,                      //Позиция окна по оси У
    g_iWindowWidth,                //Ширина окна
    g_iWindowHeight,              //Высота окна
    NULL,                    //Это наше главное окно
    NULL,                    //Нету меню
    g_hInstance,                //Дескриптор приложения
    NULL);                    //Дополнительный настроек не используем

  if(g_hWnd == NULL)                //Если не создали окно
  {
    Shutdown();
    MessageBox(NULL,"Can`t create window","Error",MB_OK|MB_ICONERROR);//Выводим сообщение
    return 0;                  //Завершаем работу приложения
  }

  if(!InitDirect3D(D3DFMT_R5G6B5,D3DFMT_D16))    //Если не смогли инициализировать Direct3D
  {
    Shutdown();
    MessageBox(NULL,"Can`t create direct3d","Error",MB_OK|MB_ICONERROR);//Выводим сообщение
    return 0;                  //Завершаем работу приложения
  }

  ShowWindow(g_hWnd,SW_SHOW);            //Отображаем окно  
  UpdateWindow(g_hWnd);              //Обновляем окно
  SetFocus(g_hWnd);                //Устанавливаем фокус на наше окно
  SetForegroundWindow(g_hWnd);          //Устанавливаем приоритет окна выше среднего

  MSG msg;
  ZeroMemory(&msg,sizeof(msg));
  
  InputObj.Initialize(g_hInstance, g_hWnd);

  LoadAllSprites();

  ExportToConsole("Class registring success!",0);
  ExportToConsole("Window create success!",0);
  ExportToConsole("D3D Creating success!",0);

  while(g_bApplicationState)            //Начинаем бесконечный цикл обработки сообщений
  {
    if(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))//Получаем сообщения
    {
      TranslateMessage(&msg);          //Обрабатываем сообщения
      DispatchMessage(&msg);          //Обрабатываем сообщения
    }
    else
      DrawFrame();              //Если сообщений нету рисуем кадры
  }

  Shutdown();                    //Освобождаем память
  return 0;                    //Завершаем работу приложения
}

long WINAPI WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
  switch(iMsg)
  {
    case WM_DESTROY:              //Если получаем сообщение о разрушении окна
    {
      g_bApplicationState = false;      //Устанавливаем состояния приложения в false (это значит что цикл обработки сообщений остановиться)
      return 0;                //Говорим виндовс что мы это сообщение обработали
    }
  }

  if(InputObj.IsKeyPressed(DIK_ESCAPE,0x80))
  {
	  ReturnToBackLayer();
	  //g_bApplicationState = false;
      return 0;
  }
  
  if(InputObj.IsKeyPressed(DIK_HOME,0xC7))
  {
	  if(ConsoleShow == false){
		ConsoleShow = true;
	  }else{
		ConsoleShow = false;
	  }
      return 0;
  }

  return DefWindowProc(hWnd,iMsg,wParam,lParam);  //Если нету для нас нужных сообщений, пусть это обрабатывает виндовс
}

bool InitDirect3D(D3DFORMAT ColorFormat,D3DFORMAT DepthFormat)
{
  if((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)//Создаем интерфейс Direct3D
    return false;                //Иначе возвращяем false

  D3DPRESENT_PARAMETERS PresParam;        //Структура с помощью которой передаем информацию устройству рендеринга при его создании
  ZeroMemory(&PresParam,sizeof(PresParam));    //Обнуляем

  HRESULT hr = NULL;                //Создаем переменную для записи в неё результатов работы функций

  D3DDISPLAYMODE DisplayMode;            //Структура для получения информации о режиме отображения в виндовс
  hr = g_pDirect3D->GetAdapterDisplayMode(    //Получаем режим отображения
    D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
    &DisplayMode);                //Записываем режим отображения в DisplayMode
  
  if(FAILED(hr))                  //Если не получилось
    return false;                //Возвращаем false

  PresParam.hDeviceWindow = g_hWnd;        //Дескриптор окна
  PresParam.Windowed = true;            //Оконный режим?
  PresParam.BackBufferWidth = g_iWindowWidth;    //Ширина заднего буфера
  PresParam.BackBufferHeight = g_iWindowHeight;  //Высота заднего буфера
  PresParam.BackBufferCount = 1;          //Количество задних буферов
  PresParam.EnableAutoDepthStencil = true;    //Используем буфер глубины и стенцил буфер
  PresParam.AutoDepthStencilFormat = DepthFormat;  //Формат буфера глубины
  PresParam.SwapEffect = D3DSWAPEFFECT_FLIP;    //Режим смены кадров
  PresParam.BackBufferFormat = DisplayMode.Format;//Устанавливаем формат пикселя определеный в виндовс

   hr = g_pDirect3D->CreateDevice(        //Создаем устройство рендеринга
    D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
    D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
    g_hWnd,                    //Дескриптор окна
    D3DCREATE_HARDWARE_VERTEXPROCESSING,    //Обрабатываем вершинны видеокартой
    &PresParam,                  //Отдаем параметры устройства
    &g_pDirect3DDevice);            //Создаем устройство рендеринга
  
  if(SUCCEEDED(hr))                //Если получилось 
    return true;                //Возвращяем true

  hr = g_pDirect3D->CreateDevice(          //Создаем устройство рендеринга
    D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
    D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
    g_hWnd,                    //Дескриптор окна
    D3DCREATE_MIXED_VERTEXPROCESSING,      //Обрабатываем вершинны смешано (видеокартой и процессором)
    &PresParam,                  //Отдаем параметры устройства
    &g_pDirect3DDevice);            //Создаем устройство рендеринга

  if(SUCCEEDED(hr))                //Если получилось
    return true;                //Возвращяем true

  hr = g_pDirect3D->CreateDevice(          //Создаем устройство рендеринга
    D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
    D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
    g_hWnd,                    //Дескриптор окна
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,    //Обрабатываем вершинны процесором
    &PresParam,                  //Отдаем параметры устройства
    &g_pDirect3DDevice);            //Создаем устройство рендеринга

  if(SUCCEEDED(hr))                //Если получилось
    return true;                //Возвращяем true

  return false;                  //Возвращяем false
}

void DrawFrame()
{
  HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();//Проверяем потерял ли Direct3DDevice устройство
  
  if(hr == D3DERR_DEVICELOST)            //Если да то
    return;                   //Выходи из функции  
  
  g_pDirect3DDevice->Clear(            //Очищаем задний буфер
    0L,                     //Размер буфера, 0 - весь буфер
    NULL,                     //Область которую будем очищать, NULL - весь буфер
    D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,     //Чистим задний буфер и буфер глубины
    D3DCOLOR_XRGB(0,0,0),            //Цвет в который очищаем задний буфер, в нашем случае черный
    1.0f,                     //Очищаем буфер глубины, заполнив его единицами
    0L);                    //Этот параметрт игнорируеться так как не выстовлен соответсвующий флаг

  g_pDirect3DDevice->BeginScene();        //Начало сцены
  InputObj.Frame();
  
  Model Fikus;
  Fikus.LoadModel("../gamedata/models/fikus.obj");
  
  if(ConsoleShow == true){
	CSprite *ConsoleSpr = new CSprite(g_pDirect3DDevice,"../gamedata/sprites/ui/console.png",g_iWindowWidth,g_iWindowHeight);
	ConsoleSpr->Render(0,0,100,40,D3DCOLOR_ARGB(255,255,255,255));
	delete ConsoleSpr;
	DrawTextD3D(g_pDirect3DDevice,hFont,"Test",0,0,1000,700,D3DCOLOR_ARGB(250,250,250,350));
  }
  CSprite *MainMenu = new CSprite(g_pDirect3DDevice,"../gamedata/sprites/ui/MainMenu.bmp",g_iWindowWidth,g_iWindowHeight);
  MainMenu->Render(0,0,100,100,D3DCOLOR_ARGB(255,255,255,255));
  delete MainMenu;
  

  
  g_pDirect3DDevice->EndScene();          //Конец сцены
  g_pDirect3DDevice->Present(NULL,NULL,NULL,NULL);//Отображаем весь задний буфер
}

void LoadAllSprites()
{
}

void Shutdown()
{
  if(g_pDirect3DDevice != NULL)          //Если мы еще не освободили интерфейс рендеринга
  {
    g_pDirect3DDevice->Release();        //То освобождаем его
    g_pDirect3DDevice = NULL;          //И устанавливаем в ноль
  }

  if(g_pDirect3D != NULL)              //Если мы еще не освободили интерфейс d3d
  {
    g_pDirect3D->Release();            //То освобождаем его
    g_pDirect3D = NULL;              //И устанавливаем в ноль
  }

  InputObj.ShutdownInput();

  if(!DestroyWindow(g_hWnd))            //Если не получилось разрушить окно
    g_hWnd = NULL;                //Устанавливаем дескриптор окна в ноль

  if(!UnregisterClass("WindowClass",g_hInstance))  //Если не получилось удалить наше зарегестрированое окно
    g_hInstance = NULL;              //Устанавливаем дескриптор приложения в ноль
}

DxInput::DxInput()
{
    m_directInput = 0;
    m_keyboard = 0;
    m_mouse = 0;
}
 
DxInput::~DxInput()
{
}
 
bool DxInput::Initialize(HINSTANCE hinstance, HWND hwnd)
{
    HRESULT result; 
 
    // Initialize the main direct input interface.
    result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
    if(FAILED(result))
    {
        return false;
    }
 
    // Initialize the direct input interface for the keyboard.
    result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
    if(FAILED(result))
    {
        return false;
    }
 
    // Set the data format.  In this case since it is a keyboard we can use the predefined data format.
    result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    if(FAILED(result))
    {
        return false;
    }
 
    /*
    // Set the cooperative level of the keyboard to not share with other programs.
    result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if(FAILED(result))
    {
        return false;
    }
    */
 
    // Now acquire the keyboard.
    result = m_keyboard->Acquire();
    if(FAILED(result))
    {
        return false;
    }
 
    // Initialize the direct input interface for the mouse.
    result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
    if(FAILED(result))
    {
        return false;
    }
 
    // Set the data format for the mouse using the pre-defined mouse data format.
    result = m_mouse->SetDataFormat(&c_dfDIMouse);
    if(FAILED(result))
    {
        return false;
    }   
 
    // Acquire the mouse.
    result = m_mouse->Acquire();
    if(FAILED(result))
    {
        return false;
    }
 
    return true;
}
        
void DxInput::ShutdownInput()
{
    // Release the mouse.
    if(m_mouse)
    {
        m_mouse->Unacquire();
        m_mouse->Release();
        m_mouse = 0;
    }
 
    // Release the keyboard.
    if(m_keyboard)
    {
        m_keyboard->Unacquire();
        m_keyboard->Release();
        m_keyboard = 0;
    }
 
    // Release the main interface to direct input.
    if(m_directInput)
    {
        m_directInput->Release();
        m_directInput = 0;
    }
}
 
bool DxInput::ReadKeyboard()
{
    HRESULT result;
 
 
    // Read the keyboard device.
    result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
    if(FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get control back.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_keyboard->Acquire();
        }
        else
        {
            return false;
        }
    }
        
    return true;
}
 
bool DxInput::ReadMouse()
{
    HRESULT result;
 
 
    // Read the mouse device.
    result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if(FAILED(result))
    {
        // If the mouse lost focus or was not acquired then try to get control back.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_mouse->Acquire();
        }
        else
        {
            return false;
        }
    }
 
    return true;
}
        
bool DxInput::Frame()
{
    bool result;
 
 
    // Read the current state of the keyboard.
    result = ReadKeyboard();
    if(!result)
    {
        return false;
    }
 
    // Read the current state of the mouse.
    result = ReadMouse();
    if(!result)
    {
        return false;
    }   
 
    return true;
}
 
bool DxInput::IsKeyPressed(int DIK,int DIK_NUMBER) 
{
    if(m_keyboardState[DIK] & DIK_NUMBER)
    {
        return true;
    }
 
    return false;
}
 
void DxInput::MouseMove(int *deltaX, int *deltaY)
{
    *deltaX = m_mouseState.lX;
    *deltaY = m_mouseState.lY;
}
 
bool DxInput::IsRMouseButtonPressed()
{
    if(m_mouseState.rgbButtons[1] & 0x80)
    {
        return true;
    }
    return false;
}
 
bool DxInput::IsLMouseButtonPressed()
{
    if(m_mouseState.rgbButtons[0] & 0x80)
    {
        return true;
    }
    return false;
}

void ReturnToBackLayer()
{
	if(CurrentLayer == "MainMenu"){
		//Здесь будет вывод в консоль
	}else{
		//char* tmp = CurrentLayer;
		//CurentLayer = BackLayer;
		//BackLayer = tmp;
	}
}

CSprite::CSprite(LPDIRECT3DDEVICE9 pD3DDevice,LPCSTR Path,UINT WindowWidth,UINT WindowHeight){
    HRESULT hr;
	WW = WindowWidth;
    WH = WindowHeight;
    D3DXGetImageInfoFromFile(Path,&tImageInfo); // загружаем инфу о картинке из файла
	hr = D3DXCreateSprite(pD3DDevice,&pSprite); // создаём pSprite
	if(FAILED(hr))
	{
		MessageBox(NULL,"Не удалось создать спрайт","Error",MB_OK|MB_ICONERROR);
		g_bApplicationState = false;
	}
    hr = D3DXCreateTextureFromFileEx(pD3DDevice,Path,tImageInfo.Width,tImageInfo.Height,
        tImageInfo.MipLevels,NULL,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255), // пиксели с этим цветом (розовый) отображаться не будут
        NULL,NULL,&pTexture); // грузим картинку из файла
	if(FAILED(hr))
	{
		MessageBox(NULL,"Не удалось создать текстуру из файла","Error",MB_OK|MB_ICONERROR);
		g_bApplicationState = false;
	}
}
 
CSprite::~CSprite(void){
    pSprite->Release(); // высвобождаем pSprite
    pTexture->Release(); // высвобождаем pTextire
    memset(&tImageInfo,0,sizeof(D3DXIMAGE_INFO)); // обнуляем структуру
}
 
void CSprite::Render(float X,float Y,float Width,float Height,D3DCOLOR Color){
    D3DXMATRIX matXY,matWH,matAll;
    D3DXMatrixTranslation(&matXY,((float)WW*X)/100,((float)WH*Y)/100,0.0f);
    D3DXMatrixScaling(&matWH,(((WW*Width)/100)/tImageInfo.Width),(((WH*Height)/100)/tImageInfo.Height),0.0f);
    D3DXMatrixMultiply(&matAll,&matWH,&matXY);
    pSprite->Begin(D3DXSPRITE_ALPHABLEND);
    pSprite->SetTransform(&matAll);
    pSprite->Draw(pTexture,NULL,NULL,NULL,Color);
    pSprite->End();
}

void DrawTextD3D(LPDIRECT3DDEVICE9 pDirect3DDevice,HFONT hFont, char* Text,int x,int y,int x1,int y1,D3DCOLOR MyColor)
{
    // Координаты прямоугольника
    Rec.left = x;                            
    Rec.top  = y;
    Rec.right  = x1;
    Rec.bottom = y1;
    // Инициализируем шрифт
    D3DXCreateFont(pDirect3DDevice, 30,10, 1, 0, FALSE, 0,30,0,0, "Arial", &pFont);    
    // Вывод текста
    pFont->DrawText(0, Text, -1, &Rec, NULL, MyColor);
}

void ExportToConsole(char* Str,int* Type)
{
	Number++;
	if(Number < 11){
		ConsStrNumb = ConsStrNumb+20;
		//wnds[Number] = CreateWindow("STATIC", Str, WS_CHILD | WS_VISIBLE, 10, ConsStrNumb, 150, 20, g_hWnd, (HMENU)Console, g_hInstance, NULL);
		if(Type == 0)
		{
			
		}
	}else{
//		wnds[1] = NULL;
		Number-1;
		for(Number;Number>0;Number-1){
//			SetWindowPos(wnds[Number],HWND_TOP,10,ConsStrNumb-20,150,20,WS_VISIBLE);
		}
	}
}

bool Mesh::LoadMesh(const std::string& Filename)
{

Assimp::Importer Importer;
const aiScene *pScene = NULL;
const aiMesh *pMesh = NULL;

pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_ValidateDataStructure | aiProcess_FindInvalidData);

if (!pScene)
{
printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
return false;
}

pMesh = pScene->mMeshes[0];
if (!pMesh)
{
printf("Error Finding Model In file.  Did you export an empty scene?");
return false;
}

for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
{
if (pMesh->mFaces[i].mNumIndices == 3)
{
m_NumIndices = m_NumIndices + 3;
}

else
{
printf("Error parsing Faces. Try to Re-Export model from 3d package!");
return false;
}
}

m_NumFaces = pMesh->mNumFaces;
m_NumVertecies = pMesh->mNumVertices;

ZeroMemory(&m_pVB, sizeof(m_pVB));m_pRenderDevice->CreateVertexBuffer(sizeof(Vertex) * m_NumVertecies, 0, VertexFVF, D3DPOOL_DEFAULT, &m_pVB, NULL);m_pVB->Lock(0, 0, (void**)&m_pVertecies, 0);

for (int i = 0; i < pMesh->mNumVertices; i++)
{
Vertex *pvertex = new Vertex(D3DXVECTOR3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z), D3DXVECTOR2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y), D3DXVECTOR3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z));
m_pVertecies[i] = pvertex;
}

m_pVB->Unlock();return true;
}
void Mesh::Render()
{
m_pRenderDevice->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
m_pRenderDevice->SetFVF(VertexFVF);
m_pRenderDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_NumFaces);
}
void Render()
{
D3DCOLOR Color = D3DCOLOR_ARGB(255, 0, 0, 255);

//Clear the Z and Back buffers
g_pRenderDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, 1.0f, 0);

g_pRenderDevice->BeginScene();InitializeViewMatrix();D3DXMATRIX Scale;
D3DXMatrixScaling(&Scale, CameraScaleX, CameraScaleY, CameraScaleZ);D3DXMATRIX Rotation;

CameraRotX += 0.025;
D3DXMatrixRotationYawPitchRoll(&Rotation, CameraRotX, CameraRotY, CameraRotZ);

g_pRenderDevice->SetTransform(D3DTS_WORLD, &D3DXMATRIX(Scale * Rotation));if (pMesh)
{
pMesh->Render();
}g_pRenderDevice->EndScene();

g_pRenderDevice->Present(NULL, NULL, NULL, NULL);
}