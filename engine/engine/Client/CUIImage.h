#pragma once
#ifndef CUIIMAGE_H
#define CUIIMAGE_H

#include "BaseUIElement.h"

class CUIImage : public BaseUIElement
{
public:
	CUIImage(int x, int y, int w, int h, const char* name);
	virtual ~CUIImage();

	void SetX(int x);
	void SetY(int y);
	void SetW(int w);
	void SetH(int h);

protected:

};

#endif