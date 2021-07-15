#pragma once
#ifndef BASEUIELEMENT_H
#define BASEUIELEMENT_H

class IRenderable;

class BaseUIElement
{
public:

	BaseUIElement(int x = 0, int y = 0, const char* texture = "", bool visability = true);
	BaseUIElement(int x = 0, int y = 0, int w = 0, int h = 0, const char* texture = "", bool visability = true);
	virtual ~BaseUIElement();

	int GetX();
	int GetY();

	int GetW();
	int GetH();

	void SetVisible(bool visability);

	void SetLayer(unsigned int layer);

protected:

	int w;
	int h;

	int x;
	int y;
	
	IRenderable* Texture;
};

#endif