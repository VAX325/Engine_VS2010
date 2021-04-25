#define _LEVEL_EDITOR
#include "../engine/engine/CBaseEntity.h"
#include "../engine/engine/LevelStructure.h"
#include "Compiler.h"
#include <fstream>

using namespace std;
#include <direct.h>

void Compiler::WriteBaseData(BaseData* BD)
{
	char* FileName = new char[512];

	//strcpy(FileName, "\\");
	strcpy(FileName, BD->LevelName);

	_mkdir(FileName);

	strcat(FileName, "\\");
	strcat(FileName, "lbd.levelbd");

	FILE* outfile;

	outfile = fopen(FileName, "w");
	
	fwrite(&(*BD), sizeof(struct BaseData), 1, outfile);

	fclose(outfile);

	_BaseDATA = *&BD;
}

void Compiler::WriteGeometry(GeometryData* GD)
{
	char* FileName = new char[512];

	//strcpy(FileName, "\\");
	strcpy(FileName, _BaseDATA->LevelName);

	//Unused return var
	void(_mkdir(FileName));

	strcat(FileName, "\\");
	strcat(FileName, "lgeom.levelgeom");

	FILE* outfile;

	outfile = fopen(FileName, "w");

	fwrite(&(*GD), sizeof(struct GeometryData), 1, outfile);

	fclose(outfile);

	_GetometryDATA = *&GD;
}

void Compiler::WriteEntitys(EntityData* ED)
{
	char* FileName = new char[512];

	//strcpy(FileName, "\\");
	strcpy(FileName, _BaseDATA->LevelName);

	_mkdir(FileName);

	strcat(FileName, "\\");
	strcat(FileName, "lent.levelent");

	FILE* outfile;

	outfile = fopen(FileName, "w");

	fwrite(&(*ED), sizeof(struct EntityData), 1, outfile);

	fclose(outfile);

	_EntityDATA = *&ED;
}
