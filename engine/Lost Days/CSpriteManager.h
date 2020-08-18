#include "CSprite.h"
#include "NVector.h"
#include "FileSystem.h"
#include <map>

typedef Vector4<float, float, float, float> FLOATVECTOR4;

class CSpriteManager
{
private:

	struct SpritesNCords
	{
		std::map<char*, CSprite*> sprites;
		std::map<CSprite*, FLOATVECTOR4 > cords;
	};

	SpritesNCords spritesNcords;

	Files txt;
	Files textures;

public:

	void RenderAllSprites();
	
	void LoadAllSprites();

	CSprite* GetSprite(char* key);

	void AddSprite(LPDIRECT3DDEVICE9 pD3DDevice, LPCSTR Path, UINT windW, UINT windH, char* key, float x, float y, float winw, float winh);
};