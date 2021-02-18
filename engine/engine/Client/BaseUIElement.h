#pragma once
#include "CSpriteManager.h"

class BaseUIElement
{
public:

	BaseUIElement(float x = 0, float y = 0, CSprite* texture = nullptr, bool visability = false);
	BaseUIElement(float x = 0, float y = 0, float w = 0, float h = 0, CSprite* texture = nullptr, bool visability = false);
	~BaseUIElement();

	float GetX();
	float GetY();

	float GetW();
	float GetH();

	void SetVisible(bool visability);
	bool GetCurrentVisability();

	void Render(LPDIRECT3DDEVICE9 pDirect3DDevice);

protected:

	float w;
	float h;

	float x;
	float y;

	bool _visability;
	CSprite* Texture;
};

