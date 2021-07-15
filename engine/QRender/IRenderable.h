#pragma once
#ifndef IRENDERABLE_H
#define IRENDERABLE_H

enum class RenderableType
{
	NONE,
	SPRITE,
};

class IRenderable
{
public:
	IRenderable() { visible = true; layer = 5; };
	virtual ~IRenderable() {};

	virtual void Render() {};
	RenderableType GetType() { return _type; };

	virtual void SetLayer(unsigned int) {};
	virtual void SetVisability(bool visible) { this->visible = visible; };

protected:
	RenderableType _type;

	int x;
	int y;

	unsigned int width;
	unsigned int height;

	unsigned int layer;
	bool visible;
};

#endif