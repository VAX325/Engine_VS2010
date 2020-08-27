#pragma once
#include "../FileSystem.h"
#include "../CLogManager.h"
#include "CSpriteManager.h"
#include "CSoundManager.h"

#include <Windows.h>

IDirect3DDevice9* GetD3D9Device();

int GetWindowH();
int GetWindowW();

HWND GetMainWnd();

CLogManager GetLogObj();
CSpriteManager GetSpriteManger();
CSoundManager GetSoundObj();
FileSystem GetFileSystemObj();