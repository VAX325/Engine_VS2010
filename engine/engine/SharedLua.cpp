#include "Client/ClientPerems.h"
#include "Server/ServerPerems.h"
#include "Client/Client.h"
#include "Main.h"
#include "SharedLua.h"
#include "Base_include.h"

bool IsClientLua(lua_State* L)
{
	return IsClient();
}

void InitLuaShared(lua_State* L, CLogManager* LogObj)
{
	luabridge::getGlobalNamespace(L)
		.beginClass<CLogManager>("CLog")
			.addFunction("Msg", &CLogManager::LogMsgLua)
			.addFunction("Error", &CLogManager::LogErrorLua)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.addFunction("GetFPS", LuaGetFPS);

	if(IsClient)
	{
		GetScriptSystemObjCl()->PushLuaObj(LogObj, (char*)"CLogClient", L);
		GetScriptSystemObjCl()->RegisterLuaObj((char*)"IsClient", IsClientLua, L);
	}
	else
	{
		GetScriptSystemObj()->PushLuaObj(LogObj, (char*)"CLogServer", L);
		GetScriptSystemObj()->RegisterLuaObj((char*)"IsClient", IsClientLua, L);
	}
}