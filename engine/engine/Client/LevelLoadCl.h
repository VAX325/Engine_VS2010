#pragma once
#include "CBaseEntity.h"
#include "LevelStructure.h"
#include <fstream>
#include <map>

class LevelFileClient
{
public:
	LevelFileClient()
	{
		_BD.LevelName = (char*)"NON_LOADED";

		Physic ph;
		ph.gravity = 0;
		_BD.PhysicData = ph;
	};

	LevelFileClient(char* LevelDir)
	{
		
	};

	CBaseEntity* GetEntity(int ID)
	{
		
	}

	~LevelFileClient() {};

private:
	BaseData _BD;
};