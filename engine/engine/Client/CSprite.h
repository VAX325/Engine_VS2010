#pragma once

#include <D3Dcommon.h>
#include <d3d9.h>
#include <d3dx9.h>

class CSprite {
private:

	LPD3DXSPRITE pSprite; // sprite �� � ������ ���� �� ����� ��������
	LPDIRECT3DTEXTURE9 pTexture; // ���� ��������
	D3DXIMAGE_INFO tImageInfo; // ��������� ���������� � ��������
	UINT WW, WH; // �������� ������� ����

	char* NameOfSprite;

public:

	CSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT WindowWidth, UINT WindowHeight, char* Name); // d3d device, ���� � �����, �������� ������� ����

	~CSprite(void); // ������� ��� sprite

	float GetW();
	float GetH();

	// ��������� float ��������� � ��������� 0 - 100
	void Render(float X, float Y, float Width, float Height, D3DCOLOR Color);
	// ��������� float ��������� � ��������� 0 - 100
	void Render(float X, float Y, float Width, float Height, D3DCOLOR Color, int rotate);

	// ��������� W � H ��� ��������� �� ������� ���������� W � H �������
	void RenderNonProcent(float X, float Y, float Width, float Height, D3DCOLOR Color);
	// ��������� W � H ��� ��������� �� ������� ���������� W � H �������
	void RenderNonProcent(float X, float Y, float Width, float Height, D3DCOLOR Color, int rotate);

	// ��������� W � H ��������� � ��������
	void RenderRect(float X, float Y, float Width, float Height, D3DCOLOR Color);
	// ��������� W � H ��������� � ��������
	void RenderRect(float X, float Y, float Width, float Height, D3DCOLOR Color, int rotate);
	
	char* GetName();
};