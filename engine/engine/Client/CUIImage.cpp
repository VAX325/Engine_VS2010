#include "Base_include.h"
#include "CUIImage.h"

CUIImage::CUIImage(int x, int y, int w, int h, const char* name) : BaseUIElement(x, y, w, h, name, true)
{

}

CUIImage::~CUIImage()
{
}

void CUIImage::SetX(int x)
{
	this->x = x;
}

void CUIImage::SetY(int y)
{
	this->y = y;
}

void CUIImage::SetW(int w)
{
	this->w = w;
}

void CUIImage::SetH(int h)
{
	this->h = h;
}
