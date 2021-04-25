#include <Base_include.h>

#include "CScriptSystem.h"

#include "Client/ClientPerems.h"
#include "Server/ServerPerems.h"

#include "Main.h"

#include <typeinfo>
#include <string>

using namespace luabridge;

#pragma comment(lib, "lua-5.2.4.lib")

CScriptSystem::CScriptSystem()
{
	
}

CScriptSystem::~CScriptSystem()
{
	
}

void CScriptSystem::LuaStart(lua_State* L)
{
	Files fls;

	if (IsClient())
	{
		fs = GetFileSystemEx();
		fls = fs->GetAllFilesInFolder((char*)"../gamedata/scripts_cl/", (char*)"sc");
	}
	else
	{
		fs = GetFileSystemEx();
		fls = fs->GetAllFilesInFolder((char*)"../gamedata/scripts_sv/", (char*)"sc");
	}

	auto it = fls.begin();

	char* buffer = (char*)malloc(512);

	while (it != fls.end())
	{
		if (IsClient())
		{
			strcpy(buffer, "../gamedata/scripts_cl/");
		}
		else
		{
			strcpy(buffer, "../gamedata/scripts_sv/");
		}

		strcat(buffer, it->second);

		Scripts[it->second] = buffer;

		luaL_dofile(L, buffer);

		ZeroMemory(buffer, 512);

		it++;
	}

	lua_pcall(L, 0, 0, 0);

	LuaRef mainLua = getGlobal(L, "Main");

	try {
		mainLua();
	}
	catch (const LuaException& e) {
		GetLogManagerEx()->LogError((char*)e.what(), true);
	}
}

void CScriptSystem::CallLuaFunc(lua_State* L, char* LuaName) 
{
	LuaRef LuaFunc = getGlobal(L, LuaName);

	try {
		LuaFunc();
	}
	catch (const LuaException& e) {
		GetLogManagerEx()->LogError((char*)e.what(), true);
	}
}

LuaFuncPtr* CScriptSystem::GetLuaFuncPtr(lua_State* L, char* LuaName)
{
	//LuaRef LuaFunc = getGlobal(L, LuaName);

	//FuncPtr ptr = LuaFunc;

	//return &ptr;

	LuaFuncPtr* _StructPtr = (LuaFuncPtr*)malloc(sizeof(LuaFuncPtr));

	_StructPtr->SetPtr(getGlobal(L, LuaName));

	//(*_StructPtr)();

	if (_StructPtr)
		return _StructPtr;
	else
		return (LuaFuncPtr*)malloc(sizeof(LuaFuncPtr));
}

void CScriptSystem::LuaUpdateCl(lua_State* L)
{
	LuaRef UpdateCl = getGlobal(L, "UpdateCl");

	try {
		UpdateCl();
	}
	catch (const LuaException& e) {
		GetLogManagerEx()->LogError((char*)e.what(), true);
	}
}

void CScriptSystem::LuaUpdate(lua_State* L)
{
	luabridge::LuaRef Update = getGlobal(L, "Update");

	try {
		Update();
	}
	catch (const LuaException& e) {
		GetLogManagerEx()->LogError((char*)e.what(), true);
	}
}