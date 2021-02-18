#pragma once

#include <D3Dcommon.h>
#include <d3d9.h>
#include <d3dx9.h>

class CSprite {
private:

	LPD3DXSPRITE pSprite; // sprite то с помощю чего мы будем рисовать
	LPDIRECT3DTEXTURE9 pTexture; // сама текстура
	D3DXIMAGE_INFO tImageInfo; // некоторая информация о картинке
	UINT WW, WH; // реальные размеры окна

	char* NameOfSprite;

public:

	CSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT WindowWidth, UINT WindowHeight, char* Name); // d3d device, путь к файлу, реальные размеры окна

	~CSprite(void); // удаляем наш sprite

	float GetW();
	float GetH();

	// параметры float указывать в процентах 0 - 100
	void Render(float X, float Y, float Width, float Height, D3DCOLOR Color);
	// параметры float указывать в процентах 0 - 100
	void Render(float X, float Y, float Width, float Height, D3DCOLOR Color, int rotate);

	// параметры W и H это множитель на который умножается W и H спрайта
	void RenderNonProcent(float X, float Y, float Width, float Height, D3DCOLOR Color);
	// параметры W и H это множитель на который умножается W и H спрайта
	void RenderNonProcent(float X, float Y, float Width, float Height, D3DCOLOR Color, int rotate);

	// Параметры W и H указывать в пикселях
	void RenderRect(float X, float Y, float Width, float Height, D3DCOLOR Color);
	// Параметры W и H указывать в пикселях
	void RenderRect(float X, float Y, float Width, float Height, D3DCOLOR Color, int rotate);
	
	char* GetName();
};