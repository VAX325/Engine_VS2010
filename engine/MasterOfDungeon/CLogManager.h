#pragma once
#include <fstream>
#include <iostream>
#include <windows.h>
#include "FileSystem.h"

using namespace std;

class CLogManager
{
public:
	
	CLogManager();
	CLogManager(const CLogManager &);
	virtual ~CLogManager();

	void LogMsg(char* Msg);

	void LogError(char* Msg, bool needToShutdown);

private:
	
	FileSystem* fs;

	ofstream fout;
};

