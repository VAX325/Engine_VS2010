#pragma once
#ifndef IRENDERABLE9_H
#define IRENDERABLE9_H

class IRenderable;
class Render9;

#include <d3dx9.h>

class IRenderable9 : public IRenderable
{
public:
	friend Render9;

	IRenderable9();
	virtual ~IRenderable9();

	virtual void Render();
	RenderableType GetType() { return _type; };
private:
	LPD3DXSPRITE pSprite;
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXIMAGE_INFO tImageInfo;
};

#endif