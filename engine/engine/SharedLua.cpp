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

const char* strcatLua(lua_State* L)
{
	//int count = lua_gettop(L); // получаем количество переданных параметров
	//if (count < 1)
	//{
	//	if (IsClient())
	//	{
	//		GetLogObjCl()->LogError((char*)"Internal LUA error", true);
	//	}
	//	else
	//	{
	//		GetLogObj()->LogError((char*)"Internal LUA error", true);
	//	}
	//}

	//const char* One = (char*)malloc(512);
	//const char* Two = (char*)malloc(512);
	//
	//One = lua_tostring(L, 1);
	//Two = lua_tostring(L, 2);

	//strcat((char*)One, Two);

	//return One;
	return "WIP";
}

void InitLuaShared(lua_State* L, CLogManager* LogObj)
{
	luabridge::getGlobalNamespace(L)
		.beginClass<CLogManager>("CLog")
		.addFunction("Msg", &CLogManager::LogMsgLua)
		.addFunction("Error", &CLogManager::LogErrorLua)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.addFunction("GetFPS", LuaGetFPS)
		.addFunction("strPlusStr", strcatLua);

	if(IsClient)
	{
		GetScriptSystemObjCl()->PushLuaObj(LogObj, (char*)"CLogClient", L);
		GetScriptSystemObjCl()->RegisterLuaObj((char*)"IsClient", IsClientLua, L);
		GetLogObjCl()->LogMsg((char*)"Pushed CLogClient!");
	}
	else
	{
		GetScriptSystemObj()->PushLuaObj(LogObj, (char*)"CLogServer", L);
		GetScriptSystemObj()->RegisterLuaObj((char*)"IsClient", IsClientLua, L);
		GetLogObjCl()->LogMsg((char*)"Pushed CLogClient!");
	}
}