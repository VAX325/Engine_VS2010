#pragma once
#include "lua_include.h"

class CLogManager;

void InitLuaShared(lua_State* L, CLogManager* LogObj);