#include "PhysicsConstants.h"
#include <Base_include.h>
#include <cmath>

World world;

void InitWorld()
{
	world.gravity.DistanceFromPlanetCore = 6371300;
	world.gravity.MassOfPlanet = 5.97;
	world.gravity.GravitationalConstant = 6.67 * pow(10, -11);

#pragma message(Reminder "AirAcceliration WIP")
	world.AirAcceliration = 0.0f;
}

World GetWorld()
{
	return world;
}