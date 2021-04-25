#pragma once
#include "BaseUIElement.h"
class CUISprite : public BaseUIElement
{
public:
	CUISprite(float x = 0, float y = 0, CSprite* texture = nullptr, bool visability = false);
	CUISprite(float x = 0, float y = 0, float w = 0, float h = 0, CSprite* texture = nullptr, bool visability = false);

	void SetW(float w);
	void SetH(float h);
};

