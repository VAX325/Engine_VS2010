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
	IRenderable() {};
	virtual ~IRenderable() {};

	virtual void Render() {};
	RenderableType GetType() { return _type; };

protected:
	RenderableType _type;

	int x;
	int y;

	unsigned int width;
	unsigned int height;
};

#endif