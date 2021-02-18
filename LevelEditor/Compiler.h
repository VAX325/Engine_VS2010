#pragma once

#include "../engine/engine/LevelStructure.h"

class Compiler
{
public:
	Compiler() { _BaseDATA = 0; };
	~Compiler() {};

	void WriteBaseData(BaseData* BD);
	void WriteEntitys();
	void BakeTextures();

private:
	BaseData* _BaseDATA;
};