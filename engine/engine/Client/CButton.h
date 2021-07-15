#pragma once
#ifndef CBUTTON_H
#define CBUTTON_H

#include "BaseUIElement.h"
#include "CUIText.h"

class LuaFuncPtr;

class CUIButton : public BaseUIElement
{
public:

	CUIButton(int x, int y, int w, int h, const char* texture);
	CUIButton(int x, int y, int w, int h, const char* texture, char* Text);
	CUIButton(int x, int y, int w, int h, const char* texture, CUIText* Text);
	CUIButton(int x, int y, int w, int h, const char* texture, std::string Text);

	CUIButton(int x, int y, int w, int h, const char* texture, LuaFuncPtr** OnClickPointer);
	CUIButton(int x, int y, int w, int h, const char* texture, char* Text, LuaFuncPtr** OnClickPointer);
	CUIButton(int x, int y, int w, int h, const char* texture, CUIText* Text, LuaFuncPtr** OnClickPointer);
	CUIButton(int x, int y, int w, int h, const char* texture, std::string Text, LuaFuncPtr** OnClickPointer);
	~CUIButton();

	bool CheckForClick(int mouseX, int mouseY);

private:
	LuaFuncPtr* clickptr;

	CUIText* text;
};

#endif