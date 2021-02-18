#pragma once
#include <map>
#include "CBaseEntity.h"
#include "LevelLoadSv.h"

class CLevelServer
{
public:
	CLevelServer();
	CLevelServer(char* LevelName);
	~CLevelServer();

	void AddEntity(CBaseEntity* Entity);
	void DeleteEntity(CBaseEntity* Entity);

	void Close();

protected:
	LevelFileServer* LF;
};