#pragma once
#include "CGameBase.h"
#include "CLevelClient.h"

class CGameCL : protected CGameBase
{
public:
	CGameCL();
	CGameCL(char* LevelName);
	~CGameCL();

	void LevelLoad(char* LevelName);

	void UnloadLevel();

private:
	CLevelClient* Level;
};

