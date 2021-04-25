#include <Base_include.h>
#include "IRenderable9.h"

IRenderable9::IRenderable9()
{

}

IRenderable9::IRenderable9(IDirect3DDevice9* device, const char* Sprite)
{
	IRenderable9::Constructor(device, Sprite);
}

IRenderable9::~IRenderable9()
{

}

void IRenderable9::Render()
{
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	pSprite->SetTransform(&matAll);

	pSprite->Draw(pTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();
}

void IRenderable9::SetX(float x)
{
	X = x;
	UpdateRect();
}

void IRenderable9::SetY(float y)
{
	Y = y;
	UpdateRect();
}

void IRenderable9::SetW(float w)
{
	W = w;
	UpdateRect();
}

void IRenderable9::SetH(float h)
{
	H = h;
	UpdateRect();
}

void IRenderable9::Constructor(IDirect3DDevice9* device, const char* Sprite)
{
	X = 0;
	Y = 0;
	W = 1;
	H = 1;

	HRESULT hr;

	D3DXGetImageInfoFromFile(Sprite, &tImageInfo); // загружаем инфу о картинке из файла
	hr = D3DXCreateSprite(device, &pSprite); // создаём pSprite

	if (FAILED(hr))
	{
		GetLogManagerEx()->LogError("Can't create sprite", true);
	}

	hr = D3DXCreateTextureFromFileEx(device, Sprite, tImageInfo.Width, tImageInfo.Height,
		tImageInfo.MipLevels, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), // пиксели с этим цветом (розовый) отображаться не будут
		NULL, NULL, &pTexture);

	//Make matrix
	D3DXMatrixTranslation(&matXY, X, Y, 0.0f);
	D3DXMatrixScaling(&matWH, W, H, 0.0f);
	D3DXMatrixMultiply(&matAll, &matWH, &matXY);

	if (FAILED(hr))
	{
		GetLogManagerEx()->LogError("Can't create texture from file", false);
	}
	else
	{
		return;
	}

	Constructor(device, "../gamedata/sprites/UI/missing.png");
}

void IRenderable9::UpdateRect()
{
	//rc = new RECT();

	D3DXMatrixTranslation(&matXY, X, Y, 0.0f);
	D3DXMatrixScaling(&matWH, W, H, 0.0f);
	D3DXMatrixMultiply(&matAll, &matWH, &matXY);
}
