#pragma once
#ifndef CLIENTPEREMS_H
#define CLIENTPEREMS_H

#include "CSoundManager.h"
#include "../QCore/FileSystem/FileSystem.h"
#include "../QRender/QRender.h"
#include "../CScriptSystem.h"
#include "../xmlparser.h"

CSoundManager* GetSoundObj();
CScriptSystem* GetScriptSystemObjCl();
CXMLParser* GetParser();

lua_State* GetLuaStateCl();
LuaFuncPtr* GetLuaFuncPtrCl(const char* LuaName);
void CallLuaFuncCl(const char* LuaFunc);

RenderManager* GetRender();

#include "../SharedLua.h"

#endif