#pragma once
#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#ifdef IS_WIN32
#ifdef IS_WIN10
//#include "../DirectX/DX12/IRenderable12.h"
class IRenderable12;
#endif
class IRenderable11;
class IRenderable9;
//#include "../DirectX/DX11/IRenderable11.h"
//#include "../DirectX/DX9/IRenderable9.h"
#endif

class QRENDER_PUBLIC IRenderable
{
public:
	IRenderable();
	~IRenderable();

#if IS_WIN32
#if IS_WIN10
	friend class DirectX12Manager;
#endif
	friend class DirectX11Manager;
	friend class DirectX9Manager;
#endif

	void SetX(float x);
	void SetY(float y);
	void SetW(float w);
	void SetH(float h);

private:
#ifdef IS_WIN32
#ifdef IS_WIN10
	IRenderable12* Render12;
#endif
	IRenderable11* Render11;
	IRenderable9* Render9;
#endif
};

#endif