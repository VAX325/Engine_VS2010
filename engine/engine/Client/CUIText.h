#pragma once

#include "BaseUIElement.h"
#include <string>

class CUIText : public BaseUIElement
{
public:
	CUIText(float w, float h);
	CUIText(float w, float h, std::string text);
	CUIText(float x, float y, float w, float h);
	CUIText(float x, float y, float w, float h, std::string text);
	~CUIText();

	void Render(LPDIRECT3DDEVICE9 pDirect3DDevice);
	void SetColor(D3DCOLOR color);
	void SetFont(LPDIRECT3DDEVICE9 pDirect3DDevice, char* FontName);

protected:
	LPD3DXFONT pFont;
	std::string Text;
	D3DCOLOR _color;
};

