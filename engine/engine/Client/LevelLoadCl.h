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
		char* Buff = (char*)malloc(512 * sizeof(Buff));
		strcpy(Buff, "../gamedata/levels/");
		strcat(Buff, LevelDir);
		strcat(Buff, "/lbd.levelbd");

		//Load file

		std::map<int, CBaseEntity*> entitys;

		EntitysAll = 0;
		for (auto it = entitys.begin(); it != entitys.end(); it++)
		{
			if (!it->second->ImPhys())
			{
				Entitys[EntitysAll] = it->second;
				EntitysAll++;
			}
		}
	};

	CBaseEntity* GetEntity(int ID)
	{
		
	}

	~LevelFileClient() {};

private:
	BaseData _BD;

	int EntitysAll;

	std::map<int, CBaseEntity*> Entitys;
};