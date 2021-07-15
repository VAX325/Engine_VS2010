#include "Base_include.h"
#include "../QRender/QRender.h"
#include "IRenderable9.h"

#include <Windows.h>
#include <d3dx9.h>

IRenderable9::IRenderable9()
{
}

IRenderable9::~IRenderable9()
{
	pSprite->Release();
	pTexture->Release();

	memset(&tImageInfo, 0, sizeof(D3DXIMAGE_INFO));
}

void IRenderable9::Render()
{
	if (!visible)
		return;

	switch (_type)
	{
	case RenderableType::NONE:
		//No render
		break;
	case RenderableType::SPRITE:
	{
		pSprite->Begin(D3DXSPRITE_ALPHABLEND);

		RECT* rct = new RECT();

		rct->left = (LONG)x;
		rct->top = (LONG)y;
		rct->right = (LONG)(x + width);
		rct->bottom = (LONG)(x + height);

		D3DXVECTOR3* pos = new D3DXVECTOR3(x, y, 0);

		pSprite->Draw(pTexture, rct, NULL, pos, D3DCOLOR_ARGB(255, 255, 255, 255));

		pSprite->End();
	}
	break;
	default:
		GetLogManagerEx()->LogError("IRenderable 0x%08x has no type!", false, this);
		break;
	}
}

void IRenderable9::SetLayer(unsigned int layer)
{
	this->layer = layer;
}
