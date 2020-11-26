#pragma once
#include "../CBaseEntity.h"
#include "NVector.h"

class CBasePhysicsEntity : private CBaseEntity
{
public:
	CBasePhysicsEntity(double mass, Vector4<float, float, float, float> CollisionPoints);
	CBasePhysicsEntity(double mass, float x1, float y1, float x2, float y2);
	~CBasePhysicsEntity();

	virtual void Update(float GravitationalConstant, float MassOfPlanet, float PlanetRadius);

	void EnableGravity(bool gravity);

private:
	
	double Mass;
	bool GravityEnabled = true;

	//x1, y1, x2, y2
	Vector4<float, float, float, float> CollisionMask;
};