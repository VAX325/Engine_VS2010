#include "Base_include.h"
#include "IRenderable.h"
#include "RenderManager.h"

#if IS_WIN32

#include <Windows.h>
#pragma comment(lib, "QCore.lib")
#pragma comment(lib, "QRenderGL.lib")
#pragma comment(lib, "QRender9.lib")
//#pragma comment(lib, "QRender11.lib")
//#pragma comment(lib, "QRender12.lib")

//#pragma comment(lib, "QRenderLegacyGL.lib")
//#pragma comment(lib, "QRenderVulkan.lib")

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
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