#pragma once
#include <Windows.h>
#include "../CLogManager.h"
#include "../FileSystem.h"
#include "../CScriptSystem.h"

CLogManager* GetLogObj();
FileSystem* GetFileSystemObj();
CScriptSystem* GetScriptSystemObj();