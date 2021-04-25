#include <Base_include.h>

#include "CGameSV.h"

CGameSV::CGameSV()
{
	Level = new CLevelServer();
	LevelLoaded = false;
}

CGameSV::CGameSV(char* LevelName)
{
	Level = new CLevelServer(LevelName);
	LevelLoaded = true;
}

CGameSV::~CGameSV()
{
	Level->Close();
	LevelLoaded = false;
}

void CGameSV::LevelLoad(char* LevelName)
{
	Level->Close();
	Level = new CLevelServer(LevelName);
	LevelLoaded = true;
}

void CGameSV::UnloadLevel()
{
	Level->Close();
	LevelLoaded = false;
}
