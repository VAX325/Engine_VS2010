#pragma once
#include "BaseUIElement.h"
#include "CUIText.h"

class LuaFuncPtr;

class CButton : public BaseUIElement
{
public:

	CButton(float x, float y, float w, float h, CSprite* texture);
	CButton(float x, float y, float w, float h, CSprite* texture, char* Text);
	CButton(float x, float y, float w, float h, CSprite* texture, CUIText* Text);
	CButton(float x, float y, float w, float h, CSprite* texture, std::string Text);

	CButton(float x, float y, float w, float h, CSprite* texture, LuaFuncPtr** OnClickPointer);
	CButton(float x, float y, float w, float h, CSprite* texture, char* Text, LuaFuncPtr** OnClickPointer);
	CButton(float x, float y, float w, float h, CSprite* texture, CUIText* Text, LuaFuncPtr** OnClickPointer);
	CButton(float x, float y, float w, float h, CSprite* texture, std::string Text, LuaFuncPtr** OnClickPointer);
	~CButton();

	bool CheckForClick(int mouseX, int mouseY);

	void Render(LPDIRECT3DDEVICE9 pDirect3DDevice);

private:
	LuaFuncPtr* clickptr;

	CUIText* text;
};
