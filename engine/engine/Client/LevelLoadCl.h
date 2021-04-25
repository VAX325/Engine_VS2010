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
		Physic ph;
		ph.gravity = 0;

		_BD = { ph, "NON_LOADED" };
	};

	LevelFileClient(char* LevelDir)
	{
		std::string Base = "../gamedata/levels/";
		Base += LevelDir;
		Base += "/";

		//BaseData
		{
			BaseData bd;

			std::string BaseData = Base + "lbd.levelbd";

			FILE* datafile;
			datafile = fopen(BaseData.c_str(), "r");
			if (datafile == NULL)
			{
				GetLogManagerEx()->LogError("Error opening lbd.levelbd file!\n", true);
			}

			// read file contents till end of file 
			while (fread(&bd, sizeof(struct BaseData), 1, datafile))
			{
				_BD = bd;
			}

		fclose(datafile);
		}
		//END

		//GeometryData
		{
			GeometryData gd;

			std::string GeomData = Base + "lgeom.levelgeom";

			FILE* geomfile;
			geomfile = fopen(GeomData.c_str(), "r");
			if (geomfile == NULL)
			{
				GetLogManagerEx()->LogError("Error opening lgeom.levelgeom file!\n", true);
				exit(1);
			}

			// read file contents till end of file 
			while (fread(&gd, sizeof(struct GeometryData), 1, geomfile))
			{
				_GD = gd;
			}

			fclose(geomfile);
		}
		//END

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

		GetLogManagerEx()->LogMsg("%s level loaded!", LevelDir);
	};

	CBaseEntity* GetEntity(int ID)
	{
		
	}

	~LevelFileClient() {};

private:
	BaseData _BD;
	GeometryData _GD;

	int EntitysAll;

	std::map<int, CBaseEntity*> Entitys;
};