#pragma once
#include <Windows.h>
#include "CLogManager.h"
#include "CSpriteManager.h"

HWND GetMainWnd();
IDirect3DDevice9* GetD3D9Device();

int GetWindowH();
int GetWindowW();

CLogManager GetLogObj();
CSpriteManager GetSpriteManger();