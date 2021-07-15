#pragma once
#include "NVector.h"
#include "CSprite.h"
#include "FileSystem.h"
#include <d3d9.h>
#include <map>

typedef VArray4<float, float, float, float> FLOATVECTOR4;

class CSpriteManager
{
private:

	struct SpritesNCords
	{
		std::map<char*, CSprite*> sprites;
		std::map<CSprite*, bool> NeedToRender;
		std::map<CSprite*, FLOATVECTOR4 > cords;
		std::map<CSprite*, int > layers;
	};

	SpritesNCords spritesNcords;

	Files txt;
	Files textures;

public:

	void RenderAllSprites();

	void LoadAllSprites();

	void SetSpriteVisible(bool visability, char* SpriteName);

	void SetSpriteVisible(bool visability, CSprite* Sprite);

	CSprite* GetSprite(char* key);

	void AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, char* key, float x, float y, bool NeedToRender, char* Name);
	/*void AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, float textureProcentW, float textureProcentH, char* key, float x, float y, bool NeedToRender);*/

	void MoveSprite(char* SpriteName, int x, int y);
	void MoveSprite(CSprite* Sprite, int x, int y);

	CSprite* GetSpriteByPos(float x, float y);
};