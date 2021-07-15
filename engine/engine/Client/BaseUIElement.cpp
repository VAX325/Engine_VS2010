#include <Base_include.h>

#include "BaseUIElement.h"
#include "ClientPerems.h"

#include "../../QRender/QRender.h"

BaseUIElement::BaseUIElement(int x, int y, const char* texture, bool visability)
{
	this->x = x;
	this->y = y;

	this->w = 0;
	this->h = 0;

	if (texture)
	{
		Texture = GetRender()->CreateRenderable(RenderableType::SPRITE, texture, x, y, w, h, NULL, NULL, 5);
		Texture->SetVisability(visability);
	}
}

BaseUIElement::BaseUIElement(int x, int y, int w, int h, const char* texture, bool visability)
{
	this->x = x;
	this->y = y;

	this->w = w;
	this->h = h;

	if(texture)
	{
		Texture = GetRender()->CreateRenderable(RenderableType::SPRITE, texture, x, y, w, h, NULL, NULL, 5);
		Texture->SetVisability(visability);
	}
}

BaseUIElement::~BaseUIElement()
{
}

void BaseUIElement::SetVisible(bool visability)
{
	if(Texture)
		Texture->SetVisability(visability);
}

void BaseUIElement::SetLayer(unsigned int layer)
{
	if(Texture)
		Texture->SetLayer(layer);
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