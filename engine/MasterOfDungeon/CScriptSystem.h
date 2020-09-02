#pragma once

#include "luabnd_include.h"

class CScriptSystem
{
public:
	
	void init();
	
	void deinit();

	int ExecuteFile(char* scriptFile);

	template<class T, typename T2>
	void Add(char* name, T pointer, T2 pointer2);

private:
	lua_State* L;
};