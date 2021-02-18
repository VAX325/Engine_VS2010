#pragma once

class CBaseEntity
{
public:
	CBaseEntity();
	CBaseEntity(double x, double y);
#ifdef _3D
	CBaseEntity(double x, double y, double z);
#endif
	~CBaseEntity();

	bool operator != (CBaseEntity another);

	virtual void Update();

protected:
	double xPos;
	double yPos;
#ifdef _3D
	double zPos;
#endif
};

