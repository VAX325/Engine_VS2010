#include "CButton.h"
#include "ClientPerems.h"
#include <Base_include.h>

CButton::CButton(float x, float y, float w, float h, CSprite* texture) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, "");
}

CButton::CButton(float x, float y, float w, float h, CSprite* texture, char* Text) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, Text);
}

CButton::CButton(float x, float y, float w, float h, CSprite* texture, CUIText* Text) : BaseUIElement(x, y, w, h, texture)
{
	text = Text;
}

CButton::CButton(float x, float y, float w, float h, CSprite* texture, std::string Text) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, Text);
}

CButton::CButton(float x, float y, float w, float h, CSprite* texture, LuaFuncPtr** OnClickPointer) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, "");
	clickptr = *OnClickPointer;
}

CButton::CButton(float x, float y, float w, float h, CSprite* texture, char* Text, LuaFuncPtr** OnClickPointer) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, Text);
	clickptr = *OnClickPointer;
}

CButton::CButton(float x, float y, float w, float h, CSprite* texture, CUIText* Text, LuaFuncPtr** OnClickPointer) : BaseUIElement(x, y, w, h, texture)
{
	text = Text;
	clickptr = *OnClickPointer;
}

CButton::CButton(float x, float y, float w, float h, CSprite* texture, std::string Text, LuaFuncPtr** OnClickPointer) : BaseUIElement(x, y, w, h, texture)
{
	text = new CUIText(x, y, w, h, Text);
	clickptr = *OnClickPointer;
}

CButton::~CButton()
{
	clickptr = 0;
}

bool CButton::CheckForClick(int mouseX, int mouseY)
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
			GetLogObjCl()->LogError((char*)e.what(), false);
		}
	}
	else
	{
		return false;
	}
	return false;
}

void CButton::Render(LPDIRECT3DDEVICE9 pDirect3DDevice)
{
	if (_visability)
	{
		if (Texture != NULL)
		{
			Texture->RenderRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		text->Render(pDirect3DDevice);
	}
}