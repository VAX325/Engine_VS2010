#pragma once
#include <map>
#include "../NVector.h"
#include "CButton.h"
#include "CUIText.h"

typedef Vector2<float, float> Cords;

class CUIPanel
{
public:

	CUIPanel(char* Name);
	~CUIPanel();

	void AddElement(CButton* button, char* text);
	void AddElement(CUIText* text1, char* text);
	void AddElement(CSprite* sprite, char* spriteName);

	CButton* GetButton(char* name);
	CUIText* GetText(char* name);

	std::map<char*, CButton*> GetAllButtons();
	std::map<char*, CUIText*> GetAllUITexts();

	void Render(LPDIRECT3DDEVICE9 pDirect3DDevice);

	char* GetName();

	void ShowPanel();
private:
	char* _Name;
	
	std::map<char*, CButton*> UIButtons;
	std::map<char*, CUIText*> UITexts;
	std::map<char*, CSprite*> UISprites;
};

