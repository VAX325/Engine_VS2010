#include <Base_include.h>

#include "Client/ClientPerems.h"
#include "Server/ServerPerems.h"
#include "Client/Client.h"
#include "Main.h"
#include "SharedLua.h"

bool IsClientLua(lua_State* L)
{
	return IsClient();
}

void LogMsgLua(lua_State* L)
{
	int count = lua_gettop(L); // получаем количество переданных параметров
	if (count < 1)
	{
		GetLogManagerEx()->LogError((char*)"Internal LUA error", true);
	}

	for (int j = 0; j < count; j++)
	{
		if (lua_isstring(L, j + 1))
		{
			char* Msg = (char*)lua_tostring(L, j + 1);

			GetLogManagerEx()->LogMsg(Msg);
		}
	}
}

void LogErrorLua(lua_State* L)
{
	int count = lua_gettop(L); // получаем количество переданных параметров
	if (count < 1)
	{
		GetLogManagerEx()->LogError((char*)"Internal LUA error", true);
	}

	char* Msg = (char*)"";
	bool needToShutdown = false;

	for (int j = 0; j < count; j++)
	{
		if (lua_isstring(L, j + 1))
		{
			Msg = (char*)lua_tostring(L, j + 1);
		}
		if (lua_isboolean(L, j + 2))
		{
			needToShutdown = lua_toboolean(L, j + 2);
		}
	}

	GetLogManagerEx()->LogError(Msg, needToShutdown);
}

void InitLuaShared(lua_State* L, CLogManager* LogObj)
{
	luabridge::getGlobalNamespace(L)
		.beginNamespace("CLog")
			.addFunction("Msg", LogMsgLua)
			.addFunction("Error", LogErrorLua)
		.endNamespace();

	luabridge::getGlobalNamespace(L)
		.addFunction("GetFPS", LuaGetFPS);
}