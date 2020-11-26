#pragma once
#include <fstream>
#include <iostream>
#include <windows.h>
#include "FileSystem.h"

using namespace std;

class CLogManager
{
public:

	void Init();

	void LogMsg(char* Msg);

	void LogError(char* Msg, bool needToShutdown);

private:
	string CurentLog;

	FileSystem* fs;

	ofstream fout;
};
