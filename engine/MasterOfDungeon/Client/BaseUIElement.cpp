#include "BaseUIElement.h"
#include "ClientPerems.h"

BaseUIElement::BaseUIElement(int x, int y, CSprite* texture)
{
	if(x && y && texture)
	{
		this->x = x;
		this->y = y;

		this->Texture = texture;

		this->w = texture->GetPosW();
		this->h = texture->GetPosH();
	}
}

BaseUIElement::~BaseUIElement()
{
	GetSpriteManger().SetSpriteVisible(false, this->Texture);

	this->x = 0;
	this->y = 0;

	this->Texture = nullptr;

	this->w = 0;
	this->h = 0;
}

int BaseUIElement::GetW()
{
	return w;
}

int BaseUIElement::GetH()
{
	return h;
}