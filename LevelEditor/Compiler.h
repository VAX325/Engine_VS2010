#pragma once

struct BaseData;
struct GeometryData;
struct EntityData;

class Compiler
{
public:
	Compiler() { _BaseDATA = 0; _GetometryDATA = 0; _EntityDATA = 0; };
	~Compiler() {};

	void WriteBaseData(BaseData* BD);
	void WriteGeometry(GeometryData* GD);
	void WriteEntitys(EntityData* ED);

private:
	BaseData* _BaseDATA;
	GeometryData* _GetometryDATA;
	EntityData* _EntityDATA;
};