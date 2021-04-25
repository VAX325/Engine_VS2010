#include <Base_include.h>
#include "CGameCL.h"

CGameCL::CGameCL()
{
	LevelLoaded = false;
	Level = new CLevelClient();
}

CGameCL::CGameCL(char* LevelName)
{
	LevelLoaded = false;
	Level = new CLevelClient(LevelName);
	Sleep(1000);
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
	LevelLoaded = false;
	Level = new CLevelClient(LevelName);
	Sleep(1000);
	LevelLoaded = true;
}

void CGameCL::UnloadLevel()
{
	Level->Close();
	LevelLoaded = false;
}
