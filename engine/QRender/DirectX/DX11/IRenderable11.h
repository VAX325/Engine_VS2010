#pragma once
#ifndef IRENDERABLE11_H
#define IRENDERABLE11_H

#pragma warning( push )
#pragma warning( disable : 4005)
#pragma warning( disable : 4838)
#include <d3d11.h>
#include <D3DX11.h>
#pragma warning( pop )

class IRenderable11
{
public:
	IRenderable11();
	IRenderable11(ID3D11Device* device, const char* Sprite, ID3D11DeviceContext* context);
	~IRenderable11();

	void Render(ID3D11DeviceContext* context);

private:
	ID3D11ShaderResourceView* texture;
};

#endif