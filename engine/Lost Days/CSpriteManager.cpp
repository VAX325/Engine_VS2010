#include "CSpriteManager.h"

void CSpriteManager::AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT windW, UINT windH, char* key, float x, float y, float winw, float winh)
{
	CSprite* Sprite = new CSprite(pD3DDevice, Path, windW, windH);
	
	spritesNcords.sprites.insert( std::make_pair(key, Sprite) );

	FLOATVECTOR4 cords = { x, y , winw, winh };

	spritesNcords.cords.insert(std::make_pair(Sprite, cords));

	delete Sprite;
}

CSprite* CSpriteManager::GetSprite(char* key)
{
	std::map <char*, CSprite*> ::iterator it;

	it = spritesNcords.sprites.find(key);

	if (it == spritesNcords.sprites.end())
		return NULL;
	else
		return it->second;
}

void CSpriteManager::RenderAllSprites()
{
	for (auto it = spritesNcords.sprites.begin(); it != spritesNcords.sprites.end(); ++it)
	{
		std::map<CSprite*, FLOATVECTOR4 > ::iterator it2;

		it2 = spritesNcords.cords.find(it->second);
		FLOATVECTOR4 v = it2->second;
		
		float x = v.first;
		float y = v.second;
		float w = v.third;
		float h = v.four;

		it->second->Render(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CSpriteManager::LoadAllSprites()
{
	
}