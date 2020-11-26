#include "Base_include.h"
#include "CUIText.h"

inline void DrawTextD3D(LPDIRECT3DDEVICE9 pDirect3DDevice, LPD3DXFONT pFont, char* Text, float x, float y, float x1, float y1, D3DCOLOR MyColor)
{
	RECT Rec;

	Rec.left = x;
	Rec.top = y;
	Rec.right = x1;
	Rec.bottom = y1;

	pFont->DrawText(0, Text, -1, &Rec, DT_CENTER, MyColor);
}

CUIText::CUIText(float w, float h) : BaseUIElement(0, 0, w, h, NULL, false)
{
	Text = "";
}

CUIText::CUIText(float w, float h, std::string text) : BaseUIElement(0, 0, w, h, NULL, false)
{
	Text = text;
}

CUIText::CUIText(float x, float y, float w, float h) : BaseUIElement(x, y, w, h, NULL, false)
{
	Text = "";
}

CUIText::CUIText(float x, float y, float w, float h, std::string text) : BaseUIElement(x, y, w, h, NULL, false)
{
	Text = text;
}

CUIText::~CUIText()
{
	Text = "";
}

void CUIText::SetFont(LPDIRECT3DDEVICE9 pDirect3DDevice, char* FontName)
{
	D3DXCreateFont(pDirect3DDevice, 30, 10, FW_NORMAL, 0, FALSE, 1, 0, 0, DEFAULT_PITCH | FF_MODERN, FontName, &pFont);
}

void CUIText::Render(LPDIRECT3DDEVICE9 pDirect3DDevice)
{
	if(GetCurrentVisability())
	{
		DrawTextD3D(pDirect3DDevice, pFont, (char*)Text.c_str(), x, y, w, h, _color);
	}
}

void CUIText::SetColor(D3DCOLOR color)
{
	_color = color;
}