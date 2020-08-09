#include <D3Dcommon.h>
#include <d3d9.h>
#include <d3dx9.h>

class CSprite {
private:

    LPD3DXSPRITE pSprite; // sprite �� � ������ ���� �� ����� ��������
    LPDIRECT3DTEXTURE9 pTexture; // ���� ��������
    D3DXIMAGE_INFO tImageInfo; // ��������� ���������� � ��������
    UINT WW, WH; // �������� ������� ����

public:

    CSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT WindowWidth, UINT WindowHeight); // d3d device, ���� � �����, �������� ������� ����

    ~CSprite(void); // ������� ��� sprite

    void Render(float X, float Y, float Width, float Height, D3DCOLOR Color); // ��������� float ��������� � ��������� 0 - 100
};