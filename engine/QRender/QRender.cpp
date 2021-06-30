#include "Base_include.h"
#include "QRender.h"

#include "../QRenderGL/RenderGL.h"
#if IS_WIN32
#include "../QRender9/Render9.h"
//#include "../QRender11/Render11.h"
#endif
#if IS_WIN10
#include "../QRender12/Render12.h"
#endif

QRENDER_PUBLIC RenderManager* InitRender(RenderEngines render_engine)
{
	int error_code = 0;
	RenderManager* manager = nullptr;

	switch (render_engine)
	{
	case RenderEngines::SYSTEM_SELECTION:
#if IS_WIN32
		//For now it opengl
		manager = CreateGLRender();
		if (manager != nullptr)
			return manager;
		else
			goto CREATION_ERROR;
#else
		manager = CreateGLRender();
		if (manager != nullptr)
			return manager;
		else
			goto CREATION_ERROR;
#endif
		break;
#if IS_WIN32
	case RenderEngines::DIRECTX9:
		manager = CreateDirect9Render();
		if (manager != nullptr)
			return manager;
		else
			goto CREATION_ERROR;
		break;
	case RenderEngines::DIRECTX11:
		/*
		manager = CreateDirect11Render();
		if (manager != nullptr)
			return manager;
		else
			goto CREATION_ERROR;
		break;
		*/
#endif
#if IS_WIN10
	case RenderEngines::DIRECTX12:
		break;
#endif
	case RenderEngines::OPENGL:
		manager = CreateGLRender();
		if (manager != nullptr)
			return manager;
		else
			goto CREATION_ERROR;
		break;
	case RenderEngines::OPENGL_LEGACY:
		break;
	case RenderEngines::VULKAN:
		break;
	default:
		break;
	}

CREATION_ERROR:
	GetLogManagerEx()->LogError("Can't create render engine! Error code: %i", true, error_code);
	return nullptr;
}