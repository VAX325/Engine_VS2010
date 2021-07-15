#include <Base_include.h>

#include "Client.h"
#include "CUIManager.h"
#include "CGameCL.h"
#include "../Server/Server.h"
#include "../xmlparser.h"
#include "../Utils.h"
#include "../../QRender/QRender.h"
#include "CInput.h"

#include <iostream>
#include <ctime>

CSoundManager* SoundManager;
CScriptSystem* ScriptSystem;
CUIManager* UIManager;
CXMLParser* xml_parser;

RenderManager* render;

lua_State* L;
CGameCL* Game;

void UICheck()
{
	if (CInput::GetInput()->IsMouseButtonUp(0))
	{
		int* cords = CInput::GetInput()->GetMouseCords();
		UIManager->GetPanel(UIManager->GetCurrentPanel())->CheckForUIEvents(cords[0], cords[1]);
	}
}

void LuaCheckForEvents()
{
}

bool Sound = false;
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
			UIManager->HidePanel("MainMenu.xml");
			UIManager->ShowPanel("LoadingScreen.xml");

			char* LevelName = (char*)lua_tostring(L, 1);
			Game = new CGameCL(LevelName);

			while (Game->IsLevelLoading())
			{
				//Here need tips
			}

			//UIManager->HidePanel("LoadingScreen.xml");
			//UIManager->ShowPanel("Game.xml");
		}
		else
		{
			GetLogManagerEx()->LogError("You atempting to load unknown level!", false);
		}
	}
	else
	{
		if (ClLaunched)
			Game->UnloadLevel();

		if (SvLaunched)
			ServerStop();

		ClLaunched = false;
		SvLaunched = false;
	}
}

void Update();
void OnShutdown();

int ClientMain(int argc, char* argv[])
{
#if IS_WIN32
#ifndef _DEBUG
	FreeConsole();
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#else
	if (::IsWindowVisible(::GetConsoleWindow()) == FALSE)
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
#endif
#endif

	InitLogManager(false);

	RenderEngines render_flag = RenderEngines::SYSTEM_SELECTION;

	for (int i = 0; i != argc; i++)
	{
		if (FindWord(argv[i], "-nosound"))
		{
			Sound = true;
			GetLogManagerEx()->LogMsg("No sound!");
		}
		if (FindWord(argv[i], "-dx11"))
		{
			render_flag = RenderEngines::DIRECTX11;
		}
		if(FindWord(argv[i], "-dx9"))
		{
			render_flag = RenderEngines::DIRECTX9;
		}
		if (FindWord(argv[i], "-gl"))
		{
			render_flag = RenderEngines::OPENGL;
		}
		if (FindWord(argv[i], "-gl_legacy"))
		{

		}
		if (FindWord(argv[i], "-vulkan"))
		{

		}
	}

	render = InitRender(render_flag);
	CInput::CreateInput(InputType::DirectInput);

	if (!Sound)
	{
		SoundManager = new CSoundManager();
		SoundManager->PlayBuffSound(0, 1.0f, true, 0.5, 1.0);
	}

	L = luaL_newstate();
	luaL_openlibs(L);

	ScriptSystem = new CScriptSystem();

	InitLuaShared(L, GetLogManagerEx());

	luabridge::getGlobalNamespace(L)
		.addFunction("CheckForEvents", LuaCheckForEvents)
		.addFunction("LevelChange", LuaLevelChange);

	ScriptSystem->LuaStart(L);

	UIManager = new CUIManager();
	UIManager->LoadPanels();
	UIManager->ShowPanel("MainMenu.xml");

	render->SetPreRender(&Update);
	render->SetShutdown(&OnShutdown);
	render->Render();

	return 0;
}

void Update()
{
	CInput::GetInput()->Update();
	UICheck();
}

void OnShutdown()
{
	Game->UnloadLevel();

	if (SvLaunched)
		ServerStop();

	SteamGameSocketsDeInit();
}

CSoundManager* GetSoundObj()
{
	return SoundManager;
}

CScriptSystem* GetScriptSystemObjCl()
{
	return ScriptSystem;
}

lua_State* GetLuaStateCl()
{
	return L;
}

void CallLuaFuncCl(const char* LuaFunc)
{
	ScriptSystem->CallLuaFunc(L, LuaFunc);
}

RenderManager* GetRender()
{
	return render;
}

LuaFuncPtr* GetLuaFuncPtrCl(const char* LuaName)
{
	LuaFuncPtr* _ptr = ScriptSystem->GetLuaFuncPtr(L, LuaName);

	return _ptr;
}

CXMLParser* GetParser()
{
	return xml_parser;
}
