#pragma once
#include "CBaseEntity.h"
#include "Server/CBasePhysicsEntity.h"
#include "LevelStructure.h"
#include <fstream>
#include <map>

class LevelFileServer
{
public:
	LevelFileServer()
	{
		_BD.LevelName = (char*)"NON_LOADED";

		Physic ph;
		ph.gravity = 0;
		_BD.PhysicData = ph;

		EntitysAll = 0;
	};

	LevelFileServer(char* LevelDir)
	{
		char* Buff = (char*)malloc(512);
		strcpy(Buff, "../gamedata/levels/");
		strcat(Buff, LevelDir);
		strcat(Buff, "/lbd.levelbd");

		std::fstream in(Buff, std::ios::binary | std::ios::in);

		BaseData bd;

		in.read((char*)&bd, sizeof(bd));
		in.close();

		_BD = bd;

		std::map<int, CBaseEntity*> entitys;

		EntitysAll = 0;
		for (auto it = entitys.begin(); it != entitys.end(); it++)
		{
			if (it->second->ImPhys())
				PhysEntitys[EntitysAll] = dynamic_cast<CBasePhysicsEntity*>(it->second);
			else
				Entitys[EntitysAll] = it->second;
			EntitysAll++;
		}
	};

	CBaseEntity* GetEntity(int ID)
	{
		if (Entitys[ID] != NULL)
			return Entitys[ID];
		else if (PhysEntitys[ID] != NULL)
			return dynamic_cast<CBaseEntity*>(PhysEntitys[ID]);
		else
			return NULL;
	}

	void AddEntity(CBaseEntity* entity)
	{
		Entitys[EntitysAll] = entity;
		EntitysAll++;
	}

	void AddEntity(CBasePhysicsEntity* entity)
	{
		PhysEntitys[EntitysAll] = entity;
		EntitysAll++;
	}

	void DeleteEntity(CBaseEntity* entity)
	{
		bool Deleted = false;

		for (auto it = Entitys.begin(); it != Entitys.end(); it++)
		{
			if (it->second == entity)
			{
				Entitys.erase(it->first);
				Deleted = true;
				break;
			}
		}

		if (!Deleted)
		{
			for (auto it = PhysEntitys.begin(); it != PhysEntitys.end(); it++)
			{
				if (it->second == entity)
				{
					PhysEntitys.erase(it->first);
					Deleted = true;
					break;
				}
			}
		}

		if (!Deleted)
		{
			printf("You atempting to delete unregistred entity");
		}
	}

	~LevelFileServer() {};

private:
	BaseData _BD;

	int EntitysAll;

	std::map<int, CBaseEntity*> Entitys;
	std::map<int, CBasePhysicsEntity*> PhysEntitys;
};