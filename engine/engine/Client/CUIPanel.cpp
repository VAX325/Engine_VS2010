#include <Base_include.h>

#include "CUIPanel.h"
#include "ClientPerems.h"

CUIPanel::CUIPanel(char* Name)
{
	_Name = Name;

	texts = 0;
	buttons = 0;
	images = 0;
}

CUIPanel::~CUIPanel()
{
}

void CUIPanel::AddElement(CUIText* text)
{
	UITexts.push_back(text);
	texts++;
}

void CUIPanel::AddElement(CUIImage* img)
{
	UIImages.push_back(img);
	images++;
}

void CUIPanel::AddElement(CUIButton* button)
{
	UIButtons.push_back(button);
	buttons++;
}

char* CUIPanel::GetName()
{
	return _Name;
}

void CUIPanel::ShowPanel()
{
	for (int i = 0; i != UITexts.size() && UITexts.size() != 0; i++)
	{
		UITexts[i]->SetVisible(true);
	}

	for (int i = 0; i != UIImages.size() && UIImages.size() != 0; i++)
	{
		UIImages[i]->SetVisible(true);
	}

	for (int i = 0; i != UIButtons.size() && UIButtons.size() != 0; i++)
	{
		UIButtons[i]->SetVisible(true);
	}
}

void CUIPanel::HidePanel()
{
	for (int i = 0; i != UITexts.size() && UITexts.size() != 0; i++)
	{
		UITexts[i]->SetVisible(false);
	}

	for (int i = 0; i != UIImages.size() && UIImages.size() != 0; i++)
	{
		UIImages[i]->SetVisible(false);
	}

	for (int i = 0; i != UIButtons.size() && UIButtons.size() != 0; i++)
	{
		UIButtons[i]->SetVisible(false);
	}
}

void CUIPanel::CheckForUIEvents(int MouseX, int MouseY)
{
	for (int i = 0; i != UIButtons.size() && UIButtons.size() != 0; i++)
	{
		UIButtons[i]->CheckForClick(MouseX, MouseY);
	}
}
