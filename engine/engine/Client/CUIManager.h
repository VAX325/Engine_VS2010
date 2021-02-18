#pragma once
#include "CUIPanel.h"
#include <map>

#define UI_NO_PANEL -1

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

	int GetCurrentPanel();

	CUIPanel* GetPanel(int panelID);

private:
	int _CurrentPanel;

	std::map<int, CUIPanel*> panels;
};

