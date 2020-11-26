#include "CButton.h"

#include <Base_include.h>

CButton::CButton(int x, int y, CSprite* texture) : BaseUIElement(x, y, texture)
{
	
}

CButton::CButton(int x, int y, CSprite* texture, static void (OnClickPointer)(void)) : BaseUIElement(x, y, texture)
{
	clickptr = *(OnClickPtr*)*(void**)(&OnClickPointer);
}

CButton::~CButton()
{
	clickptr = 0;
}

bool CButton::CheckForClick(int mouseX, int mouseY)
{
	if (mouseX > x && mouseX < x + GetW()
		&&
		mouseY > y && mouseY < y + GetH())
	{
		if(clickptr != 0)
		{
			clickptr();
		}
		return true;
	}
	else
	{
		return false;
	}
}