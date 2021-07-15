#include <Base_include.h>

#include "CButton.h"
#include "ClientPerems.h"

CUIButton::CUIButton(int x, int y, int w, int h, const char* texture) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, "");
}

CUIButton::CUIButton(int x, int y, int w, int h, const char* texture, char* Text) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, Text);
}

CUIButton::CUIButton(int x, int y, int w, int h, const char* texture, CUIText* Text) : BaseUIElement(x, y, w, h, texture)
{
	text = Text;
}

CUIButton::CUIButton(int x, int y, int w, int h, const char* texture, std::string Text) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, Text);
}

CUIButton::CUIButton(int x, int y, int w, int h, const char* texture, LuaFuncPtr** OnClickPointer) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, "");
	clickptr = *OnClickPointer;
}

CUIButton::CUIButton(int x, int y, int w, int h, const char* texture, char* Text, LuaFuncPtr** OnClickPointer) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, Text);
	clickptr = *OnClickPointer;
}

CUIButton::CUIButton(int x, int y, int w, int h, const char* texture, CUIText* Text, LuaFuncPtr** OnClickPointer) : BaseUIElement(x, y, w, h, texture)
{
	text = Text;
	clickptr = *OnClickPointer;
}

CUIButton::CUIButton(int x, int y, int w, int h, const char* texture, std::string Text, LuaFuncPtr** OnClickPointer) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, Text);
	clickptr = *OnClickPointer;
}

CUIButton::~CUIButton()
{
	clickptr = 0;
}

bool CUIButton::CheckForClick(int mouseX, int mouseY)
{
	if (mouseX > x && mouseX < x + w
		&&
		mouseY > y && mouseY < y + h)
	{
		try
		{
			LuaFuncPtr click = *clickptr;
			click();
			//CallLuaFuncCl((char*)"button_test");
			return true;
		}
		catch (const std::exception& e)
		{
			GetLogManagerEx()->LogError((char*)e.what(), false);
		}
	}
	else
	{
		return false;
	}
	return false;
}