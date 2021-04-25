#pragma once
#ifndef QRENDER_H
#define QRENDER_H

enum Specifications
{
	NOFORCE,
	OPENGL,
	D3DX9,
	D3DX11,
#ifdef IS_WIN10
	D3DX12,
#endif
	VULCAN
};

typedef void (*Function)(HWND, UINT, WPARAM, LPARAM);
QRENDER_PUBLIC void PrepareRenderEngine(int ForceToSpecification = NOFORCE, Function PreUpdate = nullptr);
QRENDER_PUBLIC void Update();
QRENDER_PUBLIC void ShutdownRenderEngine();
#include "../BaseManager.h"
QRENDER_PUBLIC GraphicsManager* GetGraphicsManager();

//Interfaces
#include "IRenderable.h"

#endif