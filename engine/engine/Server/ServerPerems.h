#pragma once
#include <Windows.h>
#include "../CScriptSystem.h"

CScriptSystem* GetScriptSystemObj();
void CallLuaFuncSv(char* LuaFunc);