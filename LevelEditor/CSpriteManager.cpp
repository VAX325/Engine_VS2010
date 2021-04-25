#include "CSpriteManager.h"
#include "Main.h"

typedef map<int, Vector2<char*, char*>> txtNtextures;

void CSpriteManager::AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, char* key, float x, float y, bool NeedToRender, char* Name)
{
	CSprite* Sprite = new CSprite(pD3DDevice, Path, GetWindowW(), GetWindowH(), Name);

	spritesNcords.sprites.insert(make_pair(key, Sprite));

	float width = Sprite->GetW();

	float height = Sprite->GetH();

	width = width / GetWindowW() * 100;

	height = height / GetWindowH() * 100;

	FLOATVECTOR4 cords = { x, y, width, height};

	spritesNcords.cords.insert(make_pair(spritesNcords.sprites[key], cords));

	spritesNcords.NeedToRender.insert(make_pair(Sprite, NeedToRender));

	//delete Sprite;
}
//
//void CSpriteManager::AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, float TextureW, float TextureH, char* key, float x, float y, bool NeedToRender)
//{
//	CSprite* Sprite = new CSprite(pD3DDevice, Path, GetWindowW(), GetWindowH());
//
//	spritesNcords.sprites.insert(make_pair(key, Sprite));
//
//	FLOATVECTOR4 cords = { x, y , TextureW, TextureH };
//
//	spritesNcords.cords.insert(make_pair(spritesNcords.sprites[key], cords));
//
//	spritesNcords.NeedToRender.insert(make_pair(Sprite, NeedToRender));
//
//	//delete Sprite;
//}

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
			spritesNcords.NeedToRender[it1->first] = visability;

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
	//return spritesNcords.sprites[key];

	/*auto it = spritesNcords.sprites.find(key);

	if (it == spritesNcords.sprites.end())
		return NULL;
	else
		return it->second;*/

	auto it = spritesNcords.sprites.begin();

	while (it != spritesNcords.sprites.end())
	{
		if (strcmp(it->first, key) != 0)
		{
			it++;
		}
		else
		{
			return it->second;
		}
	}
}

void CSpriteManager::RenderAllSprites()
{
	CSprite* spritesLayer0[256];
	CSprite* spritesLayer1[256];
	CSprite* spritesLayer2[256];
	CSprite* spritesLayer3[256];

	memset(spritesLayer0, 0, 256);
	memset(spritesLayer1, 0, 256);
	memset(spritesLayer2, 0, 256);
	memset(spritesLayer3, 0, 256);

	int firstLayer = 0;
	int secondLayer = 0;
	int thirdLayer = 0;
	int fourLayer = 0;

	auto it = spritesNcords.sprites.begin();

	while (it != spritesNcords.sprites.end())
	{
		if( spritesNcords.layers[it->second] == 0 )
		{
			spritesLayer0[firstLayer] = it->second;
			firstLayer++;
		}
		else if (spritesNcords.layers[it->second] == 1)
		{
			spritesLayer1[secondLayer] = it->second;
			secondLayer++;
		}
		else if (spritesNcords.layers[it->second] == 2)
		{
			spritesLayer2[thirdLayer] = it->second;
			thirdLayer++;
		}
		else if (spritesNcords.layers[it->second] == 3)
		{
			spritesLayer3[fourLayer] = it->second;
			fourLayer++;
		}
		it++;
	}

	int i = 0;
	while (spritesLayer0[i] != NULL)
	{
		std::map<CSprite*, bool > ::iterator it1;

		it1 = spritesNcords.NeedToRender.find(spritesLayer0[i]);

		if (it1->second)
		{
			std::map<CSprite*, FLOATVECTOR4 > ::iterator it2; 

			it2 = spritesNcords.cords.find(spritesLayer0[i]);
			FLOATVECTOR4 v = it2->second;

			float x = v.first;
			float y = v.second;
			float w = v.third;
			float h = v.four;

			spritesLayer0[i]->Render(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		i++;
	}

	i = 0;
	while (spritesLayer1[i] != NULL)
	{
		std::map<CSprite*, bool > ::iterator it1;

		it1 = spritesNcords.NeedToRender.find(spritesLayer1[i]);

		if (it1->second)
		{
			std::map<CSprite*, FLOATVECTOR4 > ::iterator it2;

			it2 = spritesNcords.cords.find(spritesLayer1[i]);
			FLOATVECTOR4 v = it2->second;

			float x = v.first;
			float y = v.second;
			float w = v.third;
			float h = v.four;

			spritesLayer1[i]->Render(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		i++;
	}

	i = 0;
	while (spritesLayer2[i] != NULL)
	{
		std::map<CSprite*, bool > ::iterator it1;

		it1 = spritesNcords.NeedToRender.find(spritesLayer2[i]);

		if (it1->second)
		{
			std::map<CSprite*, FLOATVECTOR4 > ::iterator it2;

			it2 = spritesNcords.cords.find(spritesLayer2[i]);
			FLOATVECTOR4 v = it2->second;

			float x = v.first;
			float y = v.second;
			float w = v.third;
			float h = v.four;

			spritesLayer2[i]->Render(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		i++;
	}

	i = 0;
	while (spritesLayer3[i] != NULL)
	{
		std::map<CSprite*, bool > ::iterator it1;

		it1 = spritesNcords.NeedToRender.find(spritesLayer3[i]);

		if (it1->second)
		{
			std::map<CSprite*, FLOATVECTOR4 > ::iterator it2;

			it2 = spritesNcords.cords.find(spritesLayer3[i]);
			FLOATVECTOR4 v = it2->second;

			float x = v.first;
			float y = v.second;
			float w = v.third;
			float h = v.four;

			spritesLayer3[i]->Render(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		i++;
	}
}

void CSpriteManager::LoadAllSprites()
{
	txt = GetFileSystemObjCl()->GetAllFilesInFolder((char*)"../gamedata/sprites/", (char*)"txt");

	textures = GetFileSystemObjCl()->GetAllFilesInFolder((char*)"../gamedata/sprites/", (char*)"png");

	int txtsize = txt.size();

	int texturessize = textures.size();

	if (txtsize != texturessize)
	{
		//Need to log system - complete
		GetLogObjCl()->LogError((char*)"TXT count don't match textures count", true);
	}

	auto txtit = txt.begin();
	auto textureit = textures.begin();

	txtNtextures txtNtextrs;

	int i = 0;

	while (txtit != txt.end() && textureit != textures.end())
	{

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
		char* buff1 = (char*)malloc(512);
		char* buff2 = (char*)malloc(512);

		strcpy(buff1, (CHAR*)txtNtextrsIt->second.first);
		strcpy(buff2, (CHAR*)txtNtextrsIt->second.second);

		char* txt = strtok(buff1, ".");

		char* texture = strtok(buff2, ".");

		if (strcmp(txt, texture) != 0)
		{
			GetLogObjCl()->LogError((char*)"Check your sprite files!", true);
		}

		delete buff1;
		delete buff2;

		std::string FullTXTPath = "../gamedata/sprites/" + (std::string)(CHAR*)txtNtextrsIt->second.first;

		std::string FullTexturePath = "../gamedata/sprites/" + (std::string)(CHAR*)txtNtextrsIt->second.second;

		char* bufTXT = (char*)malloc(512);

		char* buff = (char*)malloc(512);

		strcpy(bufTXT, GetFileSystemObjCl()->ReadFromFile((char*)FullTXTPath.c_str()));

		buff = strtok(bufTXT, ";");

		char* CordsNname[5];

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
				true,
				(CHAR*)txtNtextrsIt->second.second
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
				false,
				(CHAR*)txtNtextrsIt->second.second
			);
		}

		spritesNcords.layers[ spritesNcords.sprites[CordsNname[2]] ] = atoi(CordsNname[4]);

		txtNtextrsIt++;
	}
}

void CSpriteManager::MoveSprite(char* SpriteName, int x, int y)
{
	FLOATVECTOR4 old = spritesNcords.cords[GetSprite(SpriteName)];
	spritesNcords.cords[GetSprite(SpriteName)] = { (float)x, (float)y , old.third, old.four };
}

void CSpriteManager::MoveSprite(CSprite* Sprite, int x, int y)
{
	FLOATVECTOR4 old = spritesNcords.cords[Sprite];
	spritesNcords.cords[Sprite] = { (float)x, (float)y , old.third, old.four };
}

CSprite* CSpriteManager::GetSpriteByPos(float x, float y)
{
	for(auto it = spritesNcords.cords.begin(); it != spritesNcords.cords.end(); it++)
	{
		if(spritesNcords.cords[it->first].first == x && spritesNcords.cords[it->first].second == y)
		{
			return it->first;
		}
	}
	return nullptr;
}
