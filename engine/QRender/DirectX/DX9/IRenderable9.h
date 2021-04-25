#pragma once
#ifndef IRENDERABLE9_H
#define IRENDERABLE9_H

#pragma warning( push )
#pragma warning( disable : 4005)
#pragma warning( disable : 4838)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( pop )

class IRenderable9
{
public:
	IRenderable9();
	IRenderable9(IDirect3DDevice9* device, const char* Sprite);
	~IRenderable9();

	void Render();

	void SetX(float x);
	void SetY(float y);
	void SetW(float w);
	void SetH(float h);

private:
	inline void Constructor(IDirect3DDevice9* device, const char* Sprite);
	void UpdateRect();

	ID3DXSprite* pSprite;
	IDirect3DTexture9* pTexture;
	D3DXIMAGE_INFO tImageInfo;

	D3DXMATRIX matXY, matWH, matAll;

	float X;
	float Y;
	float W;
	float H;
};

#endif