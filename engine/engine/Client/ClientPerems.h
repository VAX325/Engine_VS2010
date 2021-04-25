#pragma once
#include "CSpriteManager.h"
#include "CSoundManager.h"
#include "../QCore/FileSystem/FileSystem.h"
#include "../CScriptSystem.h"

#include <Windows.h>

#include <d3dx9.h>

IDirect3DDevice9* GetD3D9Device();

int GetWindowH();
int GetWindowW();

CSpriteManager* GetSpriteManger();
CSoundManager* GetSoundObj();
CScriptSystem* GetScriptSystemObjCl();
lua_State* GetLuaStateCl();
void CallLuaFuncCl(char* LuaFunc);
LuaFuncPtr* GetLuaFuncPtrCl(char* LuaName);
HWND GetMainWnd();

#include "../SharedLua.h"