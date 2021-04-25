#pragma once
#include "CGameBase.h"
#include "CLevelClient.h"

class CGameCL : public CGameBase
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

