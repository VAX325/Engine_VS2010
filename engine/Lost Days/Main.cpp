#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    g_hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);        //������ ���������
    wc.style = CS_HREDRAW | CS_VREDRAW;      //����� ������ ����
    wc.lpfnWndProc = WndProc;              //������� ��������� ���������
    wc.cbClsExtra = 0;                //���������� ���������� ������ ��� �������� ����������
    wc.cbWndExtra = 0;                //���������� ���������� ������ ��� �������� ����������
    wc.hInstance = g_hInstance;            //���������� ����������
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //��������� ����������� ������
    wc.hCursor = LoadCursor(0, IDC_ARROW);      //��������� ����������� ������
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//���� ����� ��������� � ����� ����
    wc.lpszMenuName = 0;                //�� ���������� ����
    wc.lpszClassName = "WindowClass";            //�������� ������
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  //��������� ����������� ������

    if (!RegisterClassEx(&wc))                //������������ ����� � Windows
    {
        Shutdown();                    //����������� ������
        MessageBox(NULL, "�� �������� ���������������� ����� (WindowClass).\n �� ���. ����������� �������� ������������.", "Error", MB_OK | MB_ICONERROR); //������� ���������
        return 0;                    //��������� ������ ����������
    }

    g_hWnd = CreateWindowEx(              //������� ����
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,        //����������� ����� ����
        "WindowClass",                    //�������� ������ ����
        "Lost Days",    //�������� ����
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,//����� ����
        0,                      //������� ���� �� ��� �
        0,                      //������� ���� �� ��� �
        g_iWindowWidth,                //������ ����
        g_iWindowHeight,              //������ ����
        NULL,                    //��� ���� ������� ����
        NULL,                    //���� ����
        g_hInstance,                //���������� ����������
        NULL);                    //�������������� �������� �� ����������

    if (g_hWnd == NULL)                //���� �� ������� ����
    {
        Shutdown();
        MessageBox(NULL, "���������� ������� ����.\n �� ���. ����������� �������� ������������.", "Error", MB_OK | MB_ICONERROR);//������� ���������
        return 0;                  //��������� ������ ����������
    }

    if (!InitDirect3D(D3DFMT_R5G6B5, D3DFMT_D16))    //���� �� ������ ���������������� Direct3D
    {
        Shutdown();
        MessageBox(NULL, "���������� �������� �������� DirectX.\n �� ���. ����������� �������� ������������.", "Error", MB_OK | MB_ICONERROR);//������� ���������
        return 0;                  //��������� ������ ����������
    }

    ShowWindow(g_hWnd, SW_SHOW);            //���������� ����  
    UpdateWindow(g_hWnd);              //��������� ����
    SetFocus(g_hWnd);                //������������� ����� �� ���� ����
    SetForegroundWindow(g_hWnd);          //������������� ��������� ���� ���� ��������

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    InputObj = DxInput();

    if ( !InputObj.Initialize(g_hInstance, g_hWnd) ) 
    {
        Shutdown();
        MessageBox(NULL, "���������� �������� �������� DirectInput.\n �� ���. ����������� �������� ������������.", "Error", MB_OK | MB_ICONERROR);//������� ���������
        return 0;
    }

    while (g_bApplicationState)            //�������� ����������� ���� ��������� ���������
    {
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))//�������� ���������
        {
            TranslateMessage(&msg);          //������������ ���������
            DispatchMessage(&msg);          //������������ ���������
        }
        else
        {
            DrawFrame();              //���� ��������� ���� ������ �����
        }
    }

    Shutdown();                    //����������� ������
    return 0;                    //��������� ������ ����������
}

long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
        case WM_DESTROY:              //���� �������� ��������� � ���������� ����
        {
            g_bApplicationState = false;      //������������� ��������� ���������� � false (��� ������ ��� ���� ��������� ��������� ������������)
            return 0;                //������� ������� ��� �� ��� ��������� ����������
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

    return DefWindowProc(hWnd, iMsg, wParam, lParam);  //���� ���� ��� ��� ������ ���������, ����� ��� ������������ �������
}

bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat)
{
    if ((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)//������� ��������� Direct3D
        return false;                //����� ���������� false

    D3DPRESENT_PARAMETERS PresParam;        //��������� � ������� ������� �������� ���������� ���������� ���������� ��� ��� ��������
    ZeroMemory(&PresParam, sizeof(PresParam));    //��������

    HRESULT hr = NULL;                //������� ���������� ��� ������ � �� ����������� ������ �������

    D3DDISPLAYMODE DisplayMode;            //��������� ��� ��������� ���������� � ������ ����������� � �������
    hr = g_pDirect3D->GetAdapterDisplayMode(    //�������� ����� �����������
        D3DADAPTER_DEFAULT,              //���������� ��������� ����������
        &DisplayMode);                //���������� ����� ����������� � DisplayMode

    if (FAILED(hr))                  //���� �� ����������
        return false;                //���������� false

    PresParam.hDeviceWindow = g_hWnd;        //���������� ����
    PresParam.Windowed = true;            //������� �����?
    PresParam.BackBufferWidth = g_iWindowWidth;    //������ ������� ������
    PresParam.BackBufferHeight = g_iWindowHeight;  //������ ������� ������
    PresParam.BackBufferCount = 1;          //���������� ������ �������
    PresParam.EnableAutoDepthStencil = true;    //���������� ����� ������� � ������� �����
    PresParam.AutoDepthStencilFormat = DepthFormat;  //������ ������ �������
    PresParam.SwapEffect = D3DSWAPEFFECT_FLIP;    //����� ����� ������
    PresParam.BackBufferFormat = DisplayMode.Format;//������������� ������ ������� ����������� � �������

    hr = g_pDirect3D->CreateDevice(        //������� ���������� ����������
        D3DADAPTER_DEFAULT,              //���������� ��������� ����������
        D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
        g_hWnd,                    //���������� ����
        D3DCREATE_HARDWARE_VERTEXPROCESSING,    //������������ �������� �����������
        &PresParam,                  //������ ��������� ����������
        &g_pDirect3DDevice);            //������� ���������� ����������

    if (SUCCEEDED(hr))                //���� ���������� 
        return true;                //���������� true

    hr = g_pDirect3D->CreateDevice(          //������� ���������� ����������
        D3DADAPTER_DEFAULT,              //���������� ��������� ����������
        D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
        g_hWnd,                    //���������� ����
        D3DCREATE_MIXED_VERTEXPROCESSING,      //������������ �������� ������� (����������� � �����������)
        &PresParam,                  //������ ��������� ����������
        &g_pDirect3DDevice);            //������� ���������� ����������

    if (SUCCEEDED(hr))                //���� ����������
        return true;                //���������� true

    hr = g_pDirect3D->CreateDevice(          //������� ���������� ����������
        D3DADAPTER_DEFAULT,              //���������� ��������� ����������
        D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
        g_hWnd,                    //���������� ����
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,    //������������ �������� ����������
        &PresParam,                  //������ ��������� ����������
        &g_pDirect3DDevice);            //������� ���������� ����������

    if (SUCCEEDED(hr))                //���� ����������
        return true;                //���������� true

    return false;                  //���������� false
}

void DrawFrame()
{
    HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();//��������� ������� �� Direct3DDevice ����������

    if (hr == D3DERR_DEVICELOST)            //���� �� ��
        return;                   //������ �� �������  

    g_pDirect3DDevice->Clear(            //������� ������ �����
        0L,                     //������ ������, 0 - ���� �����
        NULL,                     //������� ������� ����� �������, NULL - ���� �����
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,     //������ ������ ����� � ����� �������
        D3DCOLOR_XRGB(0, 0, 0),            //���� � ������� ������� ������ �����, � ����� ������ ������
        1.0f,                     //������� ����� �������, �������� ��� ���������
        0L);                    //���� ��������� ������������� ��� ��� �� ��������� �������������� ����

    g_pDirect3DDevice->BeginScene();        //������ �����
    
    bool input = InputObj.Frame();

    if ( !input ) 
    {
        Shutdown();
        g_bApplicationState = false;
    }

    CSprite* MainMenu = new CSprite(g_pDirect3DDevice, "../gamedata/sprites/ui/MainMenu.bmp", g_iWindowWidth, g_iWindowHeight);
    
    MainMenu->Render(0, 0, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));

    delete MainMenu;

    g_pDirect3DDevice->EndScene();          //����� �����
    g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);//���������� ���� ������ �����
}

void Shutdown()
{
    if (g_pDirect3DDevice != NULL)          //���� �� ��� �� ���������� ��������� ����������
    {
        g_pDirect3DDevice->Release();        //�� ����������� ���
        g_pDirect3DDevice = NULL;          //� ������������� � ����
    }

    if (g_pDirect3D != NULL)              //���� �� ��� �� ���������� ��������� d3d
    {
        g_pDirect3D->Release();            //�� ����������� ���
        g_pDirect3D = NULL;              //� ������������� � ����
    }

    InputObj.ShutdownInput();

    if (!DestroyWindow(g_hWnd))            //���� �� ���������� ��������� ����
        g_hWnd = NULL;                //������������� ���������� ���� � ����

    if (!UnregisterClass("WindowClass", g_hInstance))  //���� �� ���������� ������� ���� ����������������� ����
        g_hInstance = NULL;              //������������� ���������� ���������� � ����
}

void DrawTextD3D(LPDIRECT3DDEVICE9 pDirect3DDevice, HFONT hFont, char* Text, int x, int y, int x1, int y1, D3DCOLOR MyColor)
{
    // ���������� ��������������
    Rec.left = x;
    Rec.top = y;
    Rec.right = x1;
    Rec.bottom = y1;
    // �������������� �����
    D3DXCreateFont(pDirect3DDevice, 30, 10, 1, 0, FALSE, 0, 30, 0, 0, "Arial", &pFont);
    // ����� ������
    pFont->DrawText(0, Text, -1, &Rec, NULL, MyColor);
}