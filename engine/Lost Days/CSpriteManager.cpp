#include "CSpriteManager.h"

typedef map<int, Vector2<CHAR,CHAR>  > txtNtextures;

void CSpriteManager::AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT windW, UINT windH, char* key, float x, float y, float winw, float winh)
{
	CSprite* Sprite = new CSprite(pD3DDevice, Path, windW, windH);
	
	spritesNcords.sprites.insert( make_pair(key, Sprite) );

	FLOATVECTOR4 cords = { x, y , winw, winh };

	spritesNcords.cords.insert( make_pair(Sprite, cords) );

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
	FileSystem fs = FileSystem();

	txt = fs.GetAllFilesInFolder("../gamedata/sprites/", "txt");

	textures = fs.GetAllFilesInFolder("../gamedata/sprites/", "bmp");

	if (txt.size() != textures.size())
	{
		//Need to log system
		exit(-1);
	}

	auto txtit = txt.begin();
	auto textureit = textures.begin();

	txtNtextures txtNtextrs;

	int i = 0;

	while( txtit != txt.end() && textureit != textures.end() )
	{
		if (txtit->second != textureit->second) 
		{
			exit(-1);
		}

		Vector2<CHAR, CHAR> f = { txtit->second, textureit->second };

		txtNtextrs.insert(make_pair(i, f ));

		i++;

		textureit++;
		txtit++;
	}
	
	i = 0;

	//Final step - read cord file and insert all data into maps
	while(txtNtextrs.size() <= i)
	{
		fs.ReadFromFile();

		i++;
	}
}