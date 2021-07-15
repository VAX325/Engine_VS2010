#pragma once

#include "lua_include.h"

#include "../QCore/FileSystem/FileSystem.h"

//#define RegisterLuaObjN(NameSpace, LuaName, Func) luabridge::getGlobalNamespace(L).beginNamespace(NameSpace).addFunction(LuaName, Func).endNamespace();
//#define RegisterLuaObjW(LuaName, Func) luabridge::getGlobalNamespace(L).addFunction(LuaName, Func);

class LuaFuncPtr
{
public:

	void operator()()
	{
		ref();
	}

	void SetPtr(luabridge::LuaRef _ref)
	{
		ref = _ref;
	}

	luabridge::LuaRef GetPointer()
	{
		return ref;
	}

private:
	luabridge::LuaRef ref;
};

class CScriptSystem
{
public:

	CScriptSystem();
	~CScriptSystem();

	void LuaStart(lua_State* L);
	
	void LuaUpdate(lua_State* L);
	void LuaUpdateCl(lua_State* L);
	/*void RegisterLuaObj(char* LuaName, int (* const fp)(lua_State*));*/

	template<class ReturnType, class... Params>
	void RegisterLuaObj(char* namespce, char* LuaName, ReturnType(*fp)(Params...), lua_State* L)
	{
		luabridge::Namespace Global = luabridge::getGlobalNamespace(L);

		luabridge::Namespace Namespace = Global.beginNamespace(namespce);

		Namespace.addFunction(LuaName, fp);

		Namespace.endNamespace();
		/*luabridge::getGlobalNamespace(L).
			beginNamespace(namespce)
				.addFunction((const char*)LuaName, fp)
			.endNamespace();*/
	}

	template<class ReturnType, class... Params>
	void RegisterLuaObj(char* LuaName, ReturnType(*fp)(Params...), lua_State* L)
	{
		luabridge::Namespace Global = luabridge::getGlobalNamespace(L);

		Global.addFunction(LuaName, fp);

		/*luabridge::getGlobalNamespace(L)
			.addFunction(LuaName, fp);*/
	}

	template<typename Obj>
	void PushLuaObj(Obj object, char* LuaName, lua_State* L)
	{
		luabridge::push(L, object);
		lua_setglobal(L, LuaName);
	}

	void CallLuaFunc(lua_State* L, const char* LuaName);
	LuaFuncPtr* GetLuaFuncPtr(lua_State* L, const char* LuaName);
private:

	std::map<char*, char*> Scripts;

	FileSystem* fs;
};