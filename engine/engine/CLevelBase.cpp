#include "Base_include.h"
#include "CLevelBase.h"

CLevelBase::CLevelBase()
{
	Name = (char*)"Level";
}

CLevelBase::~CLevelBase()
{
}

CLevelBase::CLevelBase(char* LevelName)
{
	Name = LevelName;
}

void CLevelBase::AddEntity(char* Name, CEntity Entity)
{
	Entitys[Name] = Entity;
}

void CLevelBase::DeleteEntity(char* Name)
{
	Entitys.erase(Name);
}

void CLevelBase::DeleteEntity(CEntity Entity)
{
	auto it = Entitys.begin();

	while(it->second != Entity)
	{
		it++;
	}

	Entitys.erase(it->first);
}