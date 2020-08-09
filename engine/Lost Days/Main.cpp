#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    g_hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);        //Размер структуры
    wc.style = CS_HREDRAW | CS_VREDRAW;      //Стили класса окна
    wc.lpfnWndProc = WndProc;              //Функция обработки сообщений
    wc.cbClsExtra = 0;                //Количество выделяемой памяти при создании приложения
    wc.cbWndExtra = 0;                //Количество выделяемой памяти при создании приложения
    wc.hInstance = g_hInstance;            //Дескриптор приложения
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //Загружаем стандартную иконку
    wc.hCursor = LoadCursor(0, IDC_ARROW);      //Загружаем стандартный курсор
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//Окно будет закрашено в белый цвет
    wc.lpszMenuName = 0;                //Не используем меню
    wc.lpszClassName = "WindowClass";            //Названия класса
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  //Загружаем стандартную иконку

    if (!RegisterClassEx(&wc))                //Регистрируем класс в Windows
    {
        Shutdown();                    //Освобождаем память
        MessageBox(NULL, "Не возможно зарегестрировать класс (WindowClass).\n За доп. информацией напишите разработчику.", "Error", MB_OK | MB_ICONERROR); //Выводим сообщение
        return 0;                    //Завершаем работу приложения
    }

    g_hWnd = CreateWindowEx(              //Создаем окно
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,        //Расширенный стиль окна
        "WindowClass",                    //Названия класса окна
        "Lost Days",    //Названия окна
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,//Стиль окна
        0,                      //Позиция окна по оси Х
        0,                      //Позиция окна по оси У
        g_iWindowWidth,                //Ширина окна
        g_iWindowHeight,              //Высота окна
        NULL,                    //Это наше главное окно
        NULL,                    //Нету меню
        g_hInstance,                //Дескриптор приложения
        NULL);                    //Дополнительный настроек не используем

    if (g_hWnd == NULL)                //Если не создали окно
    {
        Shutdown();
        MessageBox(NULL, "Невозможно создать окно.\n За доп. информацией напишите разработчику.", "Error", MB_OK | MB_ICONERROR);//Выводим сообщение
        return 0;                  //Завершаем работу приложения
    }

    if (!InitDirect3D(D3DFMT_R5G6B5, D3DFMT_D16))    //Если не смогли инициализировать Direct3D
    {
        Shutdown();
        MessageBox(NULL, "Невозможно получить контекст DirectX.\n За доп. информацией напишите разработчику.", "Error", MB_OK | MB_ICONERROR);//Выводим сообщение
        return 0;                  //Завершаем работу приложения
    }

    ShowWindow(g_hWnd, SW_SHOW);            //Отображаем окно  
    UpdateWindow(g_hWnd);              //Обновляем окно
    SetFocus(g_hWnd);                //Устанавливаем фокус на наше окно
    SetForegroundWindow(g_hWnd);          //Устанавливаем приоритет окна выше среднего

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    InputObj = DxInput();

    if ( !InputObj.Initialize(g_hInstance, g_hWnd) ) 
    {
        Shutdown();
        MessageBox(NULL, "Невозможно получить контекст DirectInput.\n За доп. информацией напишите разработчику.", "Error", MB_OK | MB_ICONERROR);//Выводим сообщение
        return 0;
    }

    while (g_bApplicationState)            //Начинаем бесконечный цикл обработки сообщений
    {
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))//Получаем сообщения
        {
            TranslateMessage(&msg);          //Обрабатываем сообщения
            DispatchMessage(&msg);          //Обрабатываем сообщения
        }
        else
        {
            DrawFrame();              //Если сообщений нету рисуем кадры
        }
    }

    Shutdown();                    //Освобождаем память
    return 0;                    //Завершаем работу приложения
}

long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
        case WM_DESTROY:              //Если получаем сообщение о разрушении окна
        {
            g_bApplicationState = false;      //Устанавливаем состояния приложения в false (это значит что цикл обработки сообщений остановиться)
            return 0;                //Говорим виндовс что мы это сообщение обработали
        }
    }

    if (InputObj.IsKeyPressed(DIK_ESCAPE, 0x01))
    {
        g_bApplicationState = false;
    }

    if (InputObj.IsKeyPressed(DIK_SEMICOLON, 0x27))
    {
        if (ConsoleShow == false) {
            ConsoleShow = true;
            //Sleep(1000);
        }
        else
        {
            ConsoleShow = false;
            //Sleep(1000);
        }
        return 0;
    }

    return DefWindowProc(hWnd, iMsg, wParam, lParam);  //Если нету для нас нужных сообщений, пусть это обрабатывает виндовс
}

bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat)
{
    if ((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)//Создаем интерфейс Direct3D
        return false;                //Иначе возвращяем false

    D3DPRESENT_PARAMETERS PresParam;        //Структура с помощью которой передаем информацию устройству рендеринга при его создании
    ZeroMemory(&PresParam, sizeof(PresParam));    //Обнуляем

    HRESULT hr = NULL;                //Создаем переменную для записи в неё результатов работы функций

    D3DDISPLAYMODE DisplayMode;            //Структура для получения информации о режиме отображения в виндовс
    hr = g_pDirect3D->GetAdapterDisplayMode(    //Получаем режим отображения
        D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
        &DisplayMode);                //Записываем режим отображения в DisplayMode

    if (FAILED(hr))                  //Если не получилось
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

    if (SUCCEEDED(hr))                //Если получилось 
        return true;                //Возвращяем true

    hr = g_pDirect3D->CreateDevice(          //Создаем устройство рендеринга
        D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
        D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
        g_hWnd,                    //Дескриптор окна
        D3DCREATE_MIXED_VERTEXPROCESSING,      //Обрабатываем вершинны смешано (видеокартой и процессором)
        &PresParam,                  //Отдаем параметры устройства
        &g_pDirect3DDevice);            //Создаем устройство рендеринга

    if (SUCCEEDED(hr))                //Если получилось
        return true;                //Возвращяем true

    hr = g_pDirect3D->CreateDevice(          //Создаем устройство рендеринга
        D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
        D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
        g_hWnd,                    //Дескриптор окна
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,    //Обрабатываем вершинны процесором
        &PresParam,                  //Отдаем параметры устройства
        &g_pDirect3DDevice);            //Создаем устройство рендеринга

    if (SUCCEEDED(hr))                //Если получилось
        return true;                //Возвращяем true

    return false;                  //Возвращяем false
}

void DrawFrame()
{
    HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();//Проверяем потерял ли Direct3DDevice устройство

    if (hr == D3DERR_DEVICELOST)            //Если да то
        return;                   //Выходи из функции  

    g_pDirect3DDevice->Clear(            //Очищаем задний буфер
        0L,                     //Размер буфера, 0 - весь буфер
        NULL,                     //Область которую будем очищать, NULL - весь буфер
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,     //Чистим задний буфер и буфер глубины
        D3DCOLOR_XRGB(0, 0, 0),            //Цвет в который очищаем задний буфер, в нашем случае черный
        1.0f,                     //Очищаем буфер глубины, заполнив его единицами
        0L);                    //Этот параметрт игнорируеться так как не выстовлен соответсвующий флаг

    g_pDirect3DDevice->BeginScene();        //Начало сцены
    
    bool input = InputObj.Frame();

    if ( !input ) 
    {
        Shutdown();
        g_bApplicationState = false;
    }

    CSprite* MainMenu = new CSprite(g_pDirect3DDevice, "../gamedata/sprites/ui/MainMenu.bmp", g_iWindowWidth, g_iWindowHeight);
    
    MainMenu->Render(0, 0, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));

    delete MainMenu;

    g_pDirect3DDevice->EndScene();          //Конец сцены
    g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);//Отображаем весь задний буфер
}

void Shutdown()
{
    if (g_pDirect3DDevice != NULL)          //Если мы еще не освободили интерфейс рендеринга
    {
        g_pDirect3DDevice->Release();        //То освобождаем его
        g_pDirect3DDevice = NULL;          //И устанавливаем в ноль
    }

    if (g_pDirect3D != NULL)              //Если мы еще не освободили интерфейс d3d
    {
        g_pDirect3D->Release();            //То освобождаем его
        g_pDirect3D = NULL;              //И устанавливаем в ноль
    }

    InputObj.ShutdownInput();

    if (!DestroyWindow(g_hWnd))            //Если не получилось разрушить окно
        g_hWnd = NULL;                //Устанавливаем дескриптор окна в ноль

    if (!UnregisterClass("WindowClass", g_hInstance))  //Если не получилось удалить наше зарегестрированое окно
        g_hInstance = NULL;              //Устанавливаем дескриптор приложения в ноль
}

void DrawTextD3D(LPDIRECT3DDEVICE9 pDirect3DDevice, HFONT hFont, char* Text, int x, int y, int x1, int y1, D3DCOLOR MyColor)
{
    // Координаты прямоугольника
    Rec.left = x;
    Rec.top = y;
    Rec.right = x1;
    Rec.bottom = y1;
    // Инициализируем шрифт
    D3DXCreateFont(pDirect3DDevice, 30, 10, 1, 0, FALSE, 0, 30, 0, 0, "Arial", &pFont);
    // Вывод текста
    pFont->DrawText(0, Text, -1, &Rec, NULL, MyColor);
}