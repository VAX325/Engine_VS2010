#pragma once
#include <map>
#include "CBaseEntity.h"
#include "LevelLoadCl.h"
#include <thread>
#include "../NET.h"

class CLevelClient
{
public:
	CLevelClient();
	CLevelClient(char* LevelName);
	CLevelClient(char* LevelName, const char* IP4port);
	~CLevelClient();

	void Close();

protected:
	LevelFileClient* LF;
	Client* client;

	std::thread ClientThread;
};

