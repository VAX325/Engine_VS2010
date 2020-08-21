#pragma once
#include "CSpriteManager.h"

class BaseUIElement
{
public:

	BaseUIElement(int x = 0, int y = 0, CSprite* texture = nullptr);
	~BaseUIElement();

	int x;
	int y;

	int GetW();
	int GetH();

private:

	int w;
	int h;

	CSprite* Texture;
};

