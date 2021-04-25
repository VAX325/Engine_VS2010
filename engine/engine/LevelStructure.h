#pragma once

class CBaseEntity;

//lbd.levelbd
struct Physic
{
	int gravity;
};

struct BaseData
{
	Physic PhysicData;

	char LevelName[120];
};
//lbd.levelbd

//lgeom.levelgeom
struct GeometryData
{
	char sprite[512][512];
	float pos[512][2];
};
//lgeom.levelgeom

//lent.levelent
struct EntityData
{
	CBaseEntity entitys[256];
	float pos[512][2];
};
//lent.levelent