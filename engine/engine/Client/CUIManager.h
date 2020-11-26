#pragma once
#include "CUIPanel.h"
#include <map>

class CUIManager
{
public:
	CUIManager();
	~CUIManager();

	void LoadPanels();

	void ShowPanel(int panelID);

	int GetCountOfPanels();

	char* GetPanelName(int panelId);

	void RenderPanels();

private:

	std::map<int, CUIPanel*> panels;
};

