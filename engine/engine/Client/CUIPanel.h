#pragma once
#ifndef CUIPANEL_H
#define CUIPANEL_H

#include "../NVarray.h"
#include "CButton.h"
#include "CUIText.h"
#include "CUIImage.h"

#include <vector>

class CUIPanel
{
public:

	CUIPanel(char* Name);
	~CUIPanel();

	void AddElement(CUIText* text);
	void AddElement(CUIImage* img);
	void AddElement(CUIButton* button);

	CUIText* GetText(int index);
	CUIImage* GetImage(int index);
	CUIButton* GetButton(int index);
	
	//std::map<int, CUIButton*> GetAllButtons();
	//std::map<int, CUIText*> GetAllUITexts();

	char* GetName();

	void ShowPanel();
	void HidePanel();

	void CheckForUIEvents(int MouseX, int MouseY);
private:
	char* _Name;
	
	int texts;
	std::vector<CUIText*> UITexts;

	int images;
	std::vector<CUIImage*> UIImages;

	int buttons;
	std::vector<CUIButton*> UIButtons;
};

#endif