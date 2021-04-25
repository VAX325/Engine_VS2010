#pragma once
#ifndef BASEMANAGER_H
#define BASEMANAGER_H

#include "Interfaces/IRenderable.h"

class GraphicsManager
{
public:
	virtual IRenderable* CreateIRenderable(const char* Sprite) { return nullptr; };

	virtual void Update() {};
	virtual void Shutdown() {};
protected:
	IRenderable* render[1024];
	int count;
};

#endif