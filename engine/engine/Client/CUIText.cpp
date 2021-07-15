#include <Base_include.h>

#include "CUIText.h"

CUIText::CUIText(int w, int h) : BaseUIElement(0, 0, w, h, NULL, false)
{
	Text = "";
}

CUIText::CUIText(int w, int h, std::string text) : BaseUIElement(0, 0, w, h, NULL, false)
{
	Text = text;
}

CUIText::CUIText(int x, int y, int w, int h) : BaseUIElement(x, y, w, h, NULL, false)
{
	Text = "";
}

CUIText::CUIText(int x, int y, int w, int h, std::string text) : BaseUIElement(x, y, w, h, NULL, false)
{
	Text = text;
}

CUIText::~CUIText()
{
	Text = "";
}

void CUIText::SetFont(const char* FontName)
{
	
}

void CUIText::SetColor(unsigned int color)
{
	
}