#pragma once
#include <d3dx9.h>
#include "CLogManager.h"

IDirect3DDevice9* GetD3D9Device();

int GetWindowH();
int GetWindowW();

CLogManager* GetLogObjCl();
FileSystem* GetFileSystemObjCl();