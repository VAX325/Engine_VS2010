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

		w = 0;
		h = 0;
	}

	_visability = visability;
}

BaseUIElement::BaseUIElement(float x, float y, float w, float h, CSprite* texture, bool visability)
{
	this->x = x;
	this->y = y;

	Texture = texture;

	this->w = w;
	this->h = h;

	_visability = visability;
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
	GetSpriteManger()->SetSpriteVisible(visability, Texture);
}

bool BaseUIElement::GetCurrentVisability()
{
	return _visability;
}

void BaseUIElement::Render(LPDIRECT3DDEVICE9 pDirect3DDevice)
{
	//if(Texture != NULL)
	//{
		//GetSpriteManger()->SetSpriteVisible(_visability, Texture);
	//}
}

int BaseUIElement::GetW()
{
	return w;
}

int BaseUIElement::GetH()
{
	return h;
}

int BaseUIElement::GetX()
{
	return x;
}

int BaseUIElement::GetY()
{
	return y;
}