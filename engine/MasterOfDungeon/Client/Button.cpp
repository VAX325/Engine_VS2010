#include "Button.h"

Button::Button(int x, int y, CSprite* texture)
{
	BaseUIElement(x, y, texture);
}

Button::~Button() 
{
	BaseUIElement::~BaseUIElement();
}

bool Button::CheckForClick(int mouseX, int mouseY)
{
	if (mouseX > this->x && mouseX < this->x + this->GetW()
		&&
		mouseY > this->y && mouseY < this->y + this->GetH())
	{
		return true;
	}
	else
	{
		return false;
	}
}