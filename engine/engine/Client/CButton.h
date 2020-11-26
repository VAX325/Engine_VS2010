#pragma once
#include "BaseUIElement.h"

class CButton : public BaseUIElement
{
public:

	CButton(int x, int y, CSprite* texture);
	CButton(int x, int y, CSprite* texture, static void (OnClickPointer)(void));
	~CButton();

	bool CheckForClick(int mouseX, int mouseY);

private:
	typedef void* (*OnClickPtr)(void);
	OnClickPtr clickptr = 0;
};
