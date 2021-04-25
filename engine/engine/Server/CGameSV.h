#pragma once
#include "CGameBase.h"
#include "CLevelServer.h"

class CGameSV : public CGameBase
{
public:
	CGameSV();
	CGameSV(char* LevelName);
	~CGameSV();

	void LevelLoad(char* LevelName);

	void UnloadLevel();

private:
	CLevelServer* Level;
};

