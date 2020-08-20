#include "CSprite.h"
#include "Perems.h" 

CSprite::CSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT WindowWidth, UINT WindowHeight) {
	HRESULT hr;

	WW = WindowWidth;
	WH = WindowHeight;

	D3DXGetImageInfoFromFile(Path, &tImageInfo); // çàãğóæàåì èíôó î êàğòèíêå èç ôàéëà
	hr = D3DXCreateSprite(pD3DDevice, &pSprite); // ñîçäà¸ì pSprite

	if (FAILED(hr))
	{
		GetLogObj().LogError("Can't create sprite", true);
	}

	hr = D3DXCreateTextureFromFileEx(pD3DDevice, Path, tImageInfo.Width, tImageInfo.Height,
		tImageInfo.MipLevels, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), // ïèêñåëè ñ ıòèì öâåòîì (ğîçîâûé) îòîáğàæàòüñÿ íå áóäóò
		NULL, NULL, &pTexture); // ãğóçèì êàğòèíêó èç ôàéëà

	if (FAILED(hr))
	{
		GetLogObj().LogError("Can't create texture from file", true);
	}
}

CSprite::~CSprite(void) {
	pSprite->Release(); // âûñâîáîæäàåì pSprite

	pTexture->Release(); // âûñâîáîæäàåì pTextire

	memset(&tImageInfo, 0, sizeof(D3DXIMAGE_INFO)); // îáíóëÿåì ñòğóêòóğó
}

void CSprite::Render(float X, float Y, float Width, float Height, D3DCOLOR Color) {
	D3DXMATRIX matXY, matWH, matAll;

	D3DXMatrixTranslation(&matXY, ((float)WW * X) / 100, ((float)WH * Y) / 100, 0.0f);
	D3DXMatrixScaling(&matWH, (((WW * Width) / 100) / tImageInfo.Width), (((WH * Height) / 100) / tImageInfo.Height), 0.0f);
	D3DXMatrixMultiply(&matAll, &matWH, &matXY);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	pSprite->SetTransform(&matAll);
	pSprite->Draw(pTexture, NULL, NULL, NULL, Color);

	pSprite->End();
}