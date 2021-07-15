#include <Base_include.h>

#include "CBasePhysicsEntity.h"
#include <cmath>

CBasePhysicsEntity::CBasePhysicsEntity(double mass, VArray4<float, float, float, float> CollisionPoints) : CBaseEntity()
{
	Mass = mass;
	CollisionMask = CollisionPoints;
}

CBasePhysicsEntity::CBasePhysicsEntity(double mass, float x1, float y1, float x2, float y2) : CBaseEntity()
{
	Mass = mass;
	CollisionMask = {x1, y1, x2, y2};
}

CBasePhysicsEntity::~CBasePhysicsEntity()
{

}

void CBasePhysicsEntity::EnableGravity(bool gravity)
{
	GravityEnabled = gravity;
}

void CBasePhysicsEntity::Update(float GravitationalConstant, float MassOfPlanet, float PlanetRadius)
{
	CBaseEntity::Update();

	if(GravityEnabled)
	{
		double GravityAcceliration = (GravitationalConstant * MassOfPlanet) / pow(PlanetRadius, 2);
	}
}