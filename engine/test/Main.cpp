#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#define TODO __FILE__ "(" $Line ") : TODO: "

//Use like: #if IS_WIN32
#define IS_WIN32 PLATFORM == PLATFORM_WINDOWS

//Use like: #if IS_MAC
#define IS_MAC PLATFORM == PLATFORM_MAC

//Use like: #if IS_UNIX
#define IS_UNIX PLATFORM == PLATFORM_UNIX

#include "../UsefullMacro.h"

#include <Windows.h>
#include "../QRender/QRender.h"

#pragma comment(lib, "QRender.lib")

#include <Windows.h>
#include <iostream>

RenderEngines render = RenderEngines::SYSTEM_SELECTION;

inline void Console(int argc, const char* argv[])
{
	if (argc > 1)
	{
		for (size_t i = 0; i < argc; i++)
		{
			if (strcmp("-render", argv[i]) == 0)
			{
				if (strcmp("dx9", argv[i + 1]) == 0)
				{
					render = RenderEngines::DIRECTX9;
				}
				else if (strcmp("dx11", argv[i + 1]) == 0)
				{
					render = RenderEngines::DIRECTX11;
				}
				else if (strcmp("gl", argv[i + 1]) == 0)
				{
					render = RenderEngines::OPENGL;
				}
				else
				{
					render = RenderEngines::OPENGL;
				}
			}
		}
	}
}

int main(int argc, const char* argv[])
{
	Console(argc, argv);

	RenderManager* render_engine = InitRender(render);
	IRenderable* rend = render_engine->CreateRenderable(RenderableType::SPRITE, "../gamedata/sprites/ENT.png", 1024/2*0, 768/2*0);

	render_engine->Render();

	return 0;
}