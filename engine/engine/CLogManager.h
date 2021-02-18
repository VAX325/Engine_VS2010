#pragma once
#include <fstream>
#include <iostream>
#include <windows.h>
#include "FileSystem.h"
#include "lua_include.h"

using namespace std;

class CLogManager
{
public:

	void Init(bool IsServer = false);

	void LogMsg(const char* Msg, ...);

	void LogError(const char* Msg, bool needToShutdown, ...);

	void LogMsgLua(lua_State* L);

	void LogErrorLua(lua_State* L);

private:
	string CurentLog;

	FileSystem* fs;

	ofstream fout;
};

CLogManager* GetLogManager();