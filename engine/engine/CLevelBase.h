#include <map>

#include "CBaseEntity.h"

typedef CBaseEntity CEntity;

class CLevelBase
{
public:
	CLevelBase();
	CLevelBase(char* LevelName);
	~CLevelBase();

	void AddEntity(char* Name, CEntity Entity);
	void DeleteEntity(char* Name);
	void DeleteEntity(CEntity Entity);

protected:
	char* Name;
	std::map<char*, CEntity> Entitys;
};