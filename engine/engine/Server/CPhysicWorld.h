#pragma once
#include <map>
#include "CBasePhysicsEntity.h"

class CPhysicWorld
{
public:

	CPhysicWorld();
	~CPhysicWorld();

	void Update();

private:
	std::map<int, CBasePhysicsEntity> PhysEntitys;
};