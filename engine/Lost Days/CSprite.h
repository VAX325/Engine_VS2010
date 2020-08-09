#include <D3Dcommon.h>
#include <d3d9.h>
#include <d3dx9.h>

class CSprite {
private:

    LPD3DXSPRITE pSprite; // sprite то с помощю чего мы будем рисовать
    LPDIRECT3DTEXTURE9 pTexture; // сама текстура
    D3DXIMAGE_INFO tImageInfo; // некоторая информация о картинке
    UINT WW, WH; // реальные размеры окна

public:

    CSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT WindowWidth, UINT WindowHeight); // d3d device, путь к файлу, реальные размеры окна

    ~CSprite(void); // удаляем наш sprite

    void Render(float X, float Y, float Width, float Height, D3DCOLOR Color); // параметры float указывать в процентах 0 - 100
};