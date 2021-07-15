#pragma once
#ifndef CUITEXT_H
#define CUITEXT_H

#include "BaseUIElement.h"
#include <string>

class CUIText : public BaseUIElement
{
public:
	CUIText(int w, int h);
	CUIText(int w, int h, std::string text);
	CUIText(int x, int y, int w, int h);
	CUIText(int x, int y, int w, int h, std::string text);
	~CUIText();

	void SetColor(unsigned int color);
	void SetFont(const char* FontName);

protected:
	std::string Text;
};

#endif