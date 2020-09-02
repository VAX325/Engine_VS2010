#pragma once
#include "CSpriteManager.h"
#include "../CLogManager.h"
#include "CSoundManager.h"
#include "../FileSystem.h"
#include "../CScriptSystem.h"

#include <Windows.h>

IDirect3DDevice9* GetD3D9Device();

int GetWindowH();
int GetWindowW();

CSpriteManager GetSpriteManger();
CLogManager GetLogObj();
CSoundManager GetSoundObj();
FileSystem GetFileSystemObj();
CScriptSystem GetScriptSystemObj();

HWND GetMainWnd();