#include "Compiler.h"
#include <fstream>

using namespace std;
#include <direct.h>

void Compiler::WriteBaseData(BaseData* BD)
{
	char* FileName = (char*)malloc(512);

	//strcpy(FileName, "\\");
	strcpy(FileName, BD->LevelName);

	_mkdir(FileName);

	strcat(FileName, "\\");
	strcat(FileName, "lbd.levelbd");

	ofstream out(FileName, ios::binary | ios::out);

	out.write((char*)&BD, sizeof(BD));

	out.close();

	_BaseDATA = *&BD;
}

void Compiler::WriteEntitys()
{

}

void Compiler::BakeTextures()
{

}
