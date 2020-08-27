#pragma once
#include "BaseUIElement.h"

class Button : BaseUIElement
{
public:

	Button(int x, int y, CSprite* texture);
	virtual ~Button();

	bool CheckForClick(int mouseX, int mouseY);

private:


};
