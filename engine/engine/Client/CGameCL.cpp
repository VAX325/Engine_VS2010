#include "Base_include.h"
#include "CGameCL.h"

CGameCL::CGameCL()
{
	Level = new CLevelClient();
	LevelLoaded = false;
}

CGameCL::CGameCL(char* LevelName)
{
	Level = new CLevelClient(LevelName);
	LevelLoaded = true;
}

CGameCL::~CGameCL()
{
	Level->Close();
	LevelLoaded = false;
}

void CGameCL::LevelLoad(char* LevelName)
{
	Level->Close();
	Level = new CLevelClient(LevelName);
	LevelLoaded = true;
}

void CGameCL::UnloadLevel()
{
	Level->Close();
	LevelLoaded = false;
}
