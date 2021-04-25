#pragma once
#ifndef CLOGMANAGER_H
#define CLOGMANAGE_H

#include <fstream>
#include <iostream>
#include <windows.h>

#include "../FileSystem/FileSystem.h"
#include "../../UsefullMacro.h"

using namespace std;

class QCORE_PUBLIC CLogManager
{
public:

	void Init(bool IsServer = false);

	void LogMsg(const char* Msg, ...);

	void LogError(const char* Msg, bool needToShutdown, ...);

private:
	string* CurentLog;

	FileSystem* fs;

	ofstream* fout;
};

QCORE_PUBLIC void InitLogManager(bool IsServer);

#ifdef QCORE_COMPILING
CLogManager* GetLogManager();
#endif

QCORE_PUBLIC CLogManager* GetLogManagerEx();

#endif