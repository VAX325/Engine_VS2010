#include <Base_include.h>
#include "Interfaces/QRender.h"
#include "../QCore/QCore.h"

#if IS_WIN32
#pragma warning( push )
#pragma warning( disable : 4005)
#include "DirectX/DirectX.h"
#pragma warning( pop )
#endif

#if IS_WIN32

#include <Windows.h>

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#else
//No Main func on unix or mac
#endif

struct Supported
{
    bool OPENGL;
#if IS_WIN32
    bool D3DX9;
    bool D3DX10;
    bool D3DX12;
#endif // IS_WIN32
    bool VULCAN;
};

inline Supported CheckForSupport()
{
    Supported support = Supported();
}

int CurrentRenderSpec = 0;
int GetCurrentRenderSpec()
{
    return CurrentRenderSpec;
}

void PrepareRenderEngine(int ForceToSpecification, Function PreUpdate)
{
    if (!IsCoreInitilaized())
        CoreInit();

    if(ForceToSpecification == OPENGL)
    {
        
    }
#if IS_WIN32
    else if(ForceToSpecification == D3DX9)
    {
        (void)InitDirectX9(PreUpdate);
    }
    else if (ForceToSpecification == D3DX11) 
    {
        (void)InitDirectX11(PreUpdate);
    }
#ifdef IS_WIN10
    else if (ForceToSpecification == D3DX12)
    {
        (void)InitDirectX12();
    }
#endif
#endif
    else if (ForceToSpecification == VULCAN)
    {

    }
    else
    {
#if IS_WIN32
        //If is windows then use directx
        CurrentRenderSpec = InitDirectX(PreUpdate);
#else
        //If is non windows then use opengl
        InitOpenGL();
#endif
    }
}

void ShutdownRenderEngine()
{
    
}

void Update()
{
    GetGraphicsManager()->Update();
}