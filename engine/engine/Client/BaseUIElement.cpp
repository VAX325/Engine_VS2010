#include <Base_include.h>
#include "BaseUIElement.h"
#include "ClientPerems.h"

BaseUIElement::BaseUIElement(float x, float y, CSprite* texture, bool visability)
{
	this->x = x;
	this->y = y;

	if(texture != NULL)
	{
		Texture = texture;

		w = texture->GetW();
		h = texture->GetH();
	}
	else
	{
		Texture = NULL;

		this->w = 0;
		this->h = 0;
	}

	_visability = visability;
	GetSpriteManger()->SetSpriteVisible(false, Texture);
}

BaseUIElement::BaseUIElement(float x, float y, float w, float h, CSprite* texture, bool visability)
{
	this->x = x;
	this->y = y;

	Texture = texture;

	this->w = w;
	this->h = h;

	_visability = visability;
	GetSpriteManger()->SetSpriteVisible(false, Texture);
}

BaseUIElement::~BaseUIElement()
{
	GetSpriteManger()->SetSpriteVisible(false, Texture);

	x = 0;
	y = 0;

	Texture = nullptr;

	w = 0;
	h = 0;
}

void BaseUIElement::SetVisible(bool visability)
{
	_visability = visability;
}

bool BaseUIElement::GetCurrentVisability()
{
	return _visability;
}

void BaseUIElement::Render(LPDIRECT3DDEVICE9 pDirect3DDevice)
{
	if(_visability)
	{
		if (Texture != NULL)
		{
			Texture->RenderRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

float BaseUIElement::GetW()
{
	return w;
}

float BaseUIElement::GetH()
{
	return h;
}

float BaseUIElement::GetX()
{
	return x;
}

float BaseUIElement::GetY()
{
	return y;
}