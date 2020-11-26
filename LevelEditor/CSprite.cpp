#include "CSprite.h"
#include "Main.h"

CSprite::CSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT WindowWidth, UINT WindowHeight, char* Name) {
	HRESULT hr;

	WW = WindowWidth;
	WH = WindowHeight;

	NameOfSprite = Name;

	D3DXGetImageInfoFromFile(Path, &tImageInfo); // загружаем инфу о картинке из файла
	hr = D3DXCreateSprite(pD3DDevice, &pSprite); // создаём pSprite

	if (FAILED(hr))
	{
		GetLogObjCl()->LogError((char*)"Can't create sprite", true);
	}

	hr = D3DXCreateTextureFromFileEx(pD3DDevice, Path, tImageInfo.Width, tImageInfo.Height,
		tImageInfo.MipLevels, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), // пиксели с этим цветом (розовый) отображаться не будут
		NULL, NULL, &pTexture); // грузим картинку из файла

	if (FAILED(hr))
	{
		GetLogObjCl()->LogError((char*)"Can't create texture from file", true);
	}
}

CSprite::~CSprite(void) {
	pSprite->Release(); // высвобождаем pSprite

	pTexture->Release(); // высвобождаем pTextire

	memset(&tImageInfo, 0, sizeof(D3DXIMAGE_INFO)); // обнуляем структуру
}

void CSprite::Render(float X, float Y, float Width, float Height, D3DCOLOR Color) {
	D3DXMATRIX matXY, matWH, matAll;
	
	D3DXMatrixTranslation(&matXY, X, Y, 0.0f);
	
	float w = (((WW * Width) / 100) / tImageInfo.Width);
	float h = (((WH * Height) / 100) / tImageInfo.Height);

	D3DXMatrixScaling(&matWH, w, h, 0.0f);
	D3DXMatrixMultiply(&matAll, &matWH, &matXY);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	pSprite->SetTransform(&matAll);

	pSprite->Draw(pTexture, NULL, NULL, NULL, Color);

	pSprite->End();
}

void CSprite::Render(float X, float Y, float Width, float Height, D3DCOLOR Color, int rotate)
{
	D3DXMATRIX matRot, matXY, matWH, matTransform, matAll;

	D3DXMatrixTranslation(&matXY, X, Y, 0.0f);
	
	float w = (((WW * Width) / 100) / tImageInfo.Width);
	float h = (((WH * Height) / 100) / tImageInfo.Height);

	D3DXMatrixScaling(&matWH, w, h, 0.0f);

	D3DXMatrixRotationZ(&matRot, (rotate * (3.1415 / 180.0)));

	D3DXMatrixMultiply(&matTransform, &matRot, &matXY);
	D3DXMatrixMultiply(&matAll, &matWH, &matTransform);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	pSprite->SetTransform(&matAll);

	pSprite->Draw(pTexture, NULL, NULL, NULL, Color);

	pSprite->End();
}

void CSprite::RenderNonProcent(float X, float Y, float Width, float Height, D3DCOLOR Color) {
	D3DXMATRIX matXY, matWH, matAll;

	D3DXMatrixTranslation(&matXY, X, Y, 0.0f);

	D3DXMatrixScaling(&matWH, Width, Height, 0.0f);

	D3DXMatrixMultiply(&matAll, &matWH, &matXY);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	pSprite->SetTransform(&matAll);

	pSprite->Draw(pTexture, NULL, NULL, NULL, Color);

	pSprite->End();
}

void CSprite::RenderNonProcent(float X, float Y, float Width, float Height, D3DCOLOR Color, int rotate)
{
	D3DXMATRIX matRot, matXY, matWH, matTransform, matAll;

	D3DXMatrixTranslation(&matXY, X, Y, 0.0f);

	D3DXMatrixScaling(&matWH, Width, Height, 0.0f);

	D3DXMatrixRotationZ(&matRot, (rotate * (3.1415 / 180.0)));

	D3DXMatrixMultiply(&matTransform, &matRot, &matXY);

	D3DXMatrixMultiply(&matAll, &matWH, &matTransform);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	pSprite->SetTransform(&matAll);

	pSprite->Draw(pTexture, NULL, NULL, NULL, Color);

	pSprite->End();
}

int CSprite::GetW() 
{
	return tImageInfo.Width;
}

int CSprite::GetH()
{
	return tImageInfo.Height;
}

char* CSprite::GetName()
{
	return NameOfSprite;
}