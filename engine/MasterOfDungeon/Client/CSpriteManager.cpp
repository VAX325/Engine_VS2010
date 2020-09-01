#include "CSpriteManager.h"
#include "ClientPerems.h"

typedef map<int, Vector2<char*, char*>  > txtNtextures;

void CSpriteManager::AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, char* key, float x, float y, bool NeedToRender)
{
	CSprite* Sprite = new CSprite(pD3DDevice, Path, GetWindowW(), GetWindowH());

	spritesNcords.sprites.insert(make_pair(key, Sprite));

	FLOATVECTOR4 cords = { x, y , 100, 100 };

	spritesNcords.cords.insert(make_pair(spritesNcords.sprites[key], cords));

	spritesNcords.NeedToRender.insert(make_pair(Sprite, NeedToRender));

	//delete Sprite;
}

void CSpriteManager::AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT TextureW, UINT TextureH, char* key, float x, float y, bool NeedToRender)
{
	CSprite* Sprite = new CSprite(pD3DDevice, Path, TextureW, TextureH);

	spritesNcords.sprites.insert(make_pair(key, Sprite));

	FLOATVECTOR4 cords = { x, y , 100, 100 };

	spritesNcords.cords.insert(make_pair(spritesNcords.sprites[key], cords));

	spritesNcords.NeedToRender.insert(make_pair(Sprite, NeedToRender));

	//delete Sprite;
}

void CSpriteManager::SetSpriteVisible(bool visability,char* SpriteName)
{
	CSprite* spriteobj = GetSprite(SpriteName);

	auto it = spritesNcords.sprites.begin();

	bool TaskComplete = false;

	while (it != spritesNcords.sprites.end() && !TaskComplete)
	{
		std::map<CSprite*, bool > ::iterator it1;

		it1 = spritesNcords.NeedToRender.find(it->second);

		if (it->second == spriteobj)
		{
			spritesNcords.NeedToRender[(CSprite*)it1->second] = visability;

			TaskComplete = true;
		}
		else
		{
			++it;
		}
	}
}

void CSpriteManager::SetSpriteVisible(bool visability, CSprite* Sprite) 
{
	auto it = spritesNcords.sprites.begin();

	bool TaskComplete = false;

	while (it != spritesNcords.sprites.end() && !TaskComplete)
	{
		std::map<CSprite*, bool > ::iterator it1;

		it1 = spritesNcords.NeedToRender.find(it->second);

		if (it->second == Sprite)
		{
			spritesNcords.NeedToRender[(CSprite*)it1->second] = visability;

			TaskComplete = true;
		}
		else
		{
			++it;
		}
	}
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
		std::map<CSprite*, bool > ::iterator it1;

		it1 = spritesNcords.NeedToRender.find(it->second);

		if(it1->second)
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
}

void CSpriteManager::LoadAllSprites()
{
	txt = GetFileSystemObj().GetAllFilesInFolder("../gamedata/sprites/", "txt");

	textures = GetFileSystemObj().GetAllFilesInFolder("../gamedata/sprites/", "bmp");

	int txtsize = txt.size();

	int texturessize = textures.size();

	if (txtsize != texturessize)
	{
		//Need to log system - complete
		GetLogObj().LogError("TXT count don't match textures count", true);
	}

	auto txtit = txt.begin();
	auto textureit = textures.begin();

	txtNtextures txtNtextrs;

	int i = 0;

	while (txtit != txt.end() && textureit != textures.end())
	{
		//char* buffTXT = strtok(txtit->second, ".");

		//char* buffTextrs = strtok(textureit->second, ".");

		//Это не работает - переделай
		//if (buffTXT != buffTextrs)
		//{
			//printf("Bye\n");
			//exit(-1);
		//}

		Vector2<char*, char*> f = { txtit->second, textureit->second };

		txtNtextrs.insert(make_pair(i, f));

		i++;

		textureit++;
		txtit++;
	}

	//Final step - read cord file and insert all data into maps

	auto txtNtextrsIt = txtNtextrs.begin();

	while (txtNtextrsIt != txtNtextrs.end())
	{
		std::string FullTXTPath = "../gamedata/sprites/" + (std::string)(CHAR*)txtNtextrsIt->second.first;

		std::string FullTexturePath = "../gamedata/sprites/" + (std::string)(CHAR*)txtNtextrsIt->second.second;

		char* bufTXT = (char*)malloc(512);

		char* buff = (char*)malloc(512);

		strcpy(bufTXT, GetFileSystemObj().ReadFromFile((char*)FullTXTPath.c_str()));

		buff = strtok(bufTXT, ";");

		char* CordsNname[4];

		int i = 0;

		while (buff != NULL)
		{
			CordsNname[i] = buff;
			buff = strtok(NULL, ";");
			i++;
		}

		if( strcmp(CordsNname[3], "true") == 0 )
		{
			AddSprite(
				GetD3D9Device(),
				FullTexturePath.c_str(),
				CordsNname[2],
				atof(CordsNname[0]),
				atof(CordsNname[1]),
				true
			);
		}
		else
		{
			AddSprite(
				GetD3D9Device(),
				FullTexturePath.c_str(),
				CordsNname[2],
				atof(CordsNname[0]),
				atof(CordsNname[1]),
				false
			);
		}
		txtNtextrsIt++;
	}
}