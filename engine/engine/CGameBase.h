#pragma once

class CGameBase
{
public:
	void Start() {};

	void Tick() {};

	void End() {};

	void LoadLevel(char* LevelName) {};

	bool IsLevelLoaded() { return LevelLoaded; };
protected:

	bool LevelLoaded;
};

