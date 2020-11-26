#pragma once
#include "CSpriteManager.h"
#include "../CLogManager.h"
#include "CSoundManager.h"
#include "../FileSystem.h"
#include "../CScriptSystem.h"

#include <Windows.h>

#include <d3dx9.h>

IDirect3DDevice9* GetD3D9Device();

int GetWindowH();
int GetWindowW();

CSpriteManager* GetSpriteManger();
CLogManager* GetLogObjCl();
CSoundManager* GetSoundObj();
FileSystem* GetFileSystemObjCl();
CScriptSystem* GetScriptSystemObjCl();
lua_State* GetLuaStateCl();
HWND GetMainWnd();

#include "../SharedLua.h"