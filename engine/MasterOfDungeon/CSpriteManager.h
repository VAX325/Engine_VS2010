#pragma once
#include "CSprite.h"
#include "NVector.h"
#include "FileSystem.h"
#include <d3d9.h>
#include <map>

typedef Vector4<float, float, float, float> FLOATVECTOR4;

class CSpriteManager
{
private:

	struct SpritesNCords
	{
		std::map<char*, CSprite*> sprites;
		std::map<CSprite*, bool> NeedToRender;
		std::map<CSprite*, FLOATVECTOR4 > cords;
	};

	SpritesNCords spritesNcords;

	Files txt;
	Files textures;

public:

	void RenderAllSprites();

	void LoadAllSprites();

	void SetSpriteVisible(bool visability,char* SpriteName);

	void SetSpriteVisible(bool visability, CSprite* Sprite);

	CSprite* GetSprite(char* key);

	void AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, char* key, float x, float y, bool NeedToRender);
	void AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT windW, UINT windH, char* key, float x, float y, bool NeedToRender);
};