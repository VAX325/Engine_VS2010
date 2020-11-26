#include "Base_include.h"
#include "CUIPanel.h"
#include "ClientPerems.h"

CUIPanel::CUIPanel(char* Name)
{
	_Name = Name;
}

CUIPanel::~CUIPanel()
{

}

void CUIPanel::AddElement(CButton* button, char* text)
{
	UIButtons[text] = button;
}


void CUIPanel::AddElement(CUIText* text1, char* text)
{
	UITexts[text] = text1;
}

void CUIPanel::AddElement(CSprite* sprite, char* spriteName)
{
	UISprites[spriteName] = sprite;
}

std::map<char*, CButton*> CUIPanel::GetAllButtons()
{
	return UIButtons;
}

std::map<char*, CUIText*> CUIPanel::GetAllUITexts()
{
	return UITexts;
}

void CUIPanel::Render(LPDIRECT3DDEVICE9 pDirect3DDevice)
{
	for(auto it = UIButtons.begin(); it != UIButtons.end(); it++)
	{
		it->second->Render(pDirect3DDevice);
	}

	for (auto it = UITexts.begin(); it != UITexts.end(); it++)
	{
		it->second->Render(pDirect3DDevice);
	}
}

char* CUIPanel::GetName()
{
	return _Name;
}

void CUIPanel::ShowPanel()
{
	for (auto buttonsit = UIButtons.begin(); buttonsit != UIButtons.end(); buttonsit++)
	{
		buttonsit->second->SetVisible(true);
	}

	for (auto spriteit = UISprites.begin(); spriteit != UISprites.end(); spriteit++)
	{
		GetSpriteManger()->SetSpriteVisible(true, spriteit->second);
	}

	for (auto textit = UITexts.begin(); textit != UITexts.end(); textit++)
	{
		textit->second->SetVisible(true);
	}
}
