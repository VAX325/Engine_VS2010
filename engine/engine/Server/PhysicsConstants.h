#pragma once

struct World
{	
	struct Gravity
	{
		//In MassOfPlanet * 10²⁴
		float MassOfPlanet;
		float DistanceFromPlanetCore;
		float GravitationalConstant;
	};

	Gravity gravity;

	float AirAcceliration;
};

void InitWorld();
World GetWorld();