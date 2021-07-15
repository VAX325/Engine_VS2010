#pragma once
#ifndef CUIMANAGER_H
#define CUIMANAGER_H

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
	void HidePanel(int panelID);

	void ShowPanel(const char* panelName);
	void HidePanel(const char* panelName);

	int GetCountOfPanels();

	char* GetPanelName(int panelId);

	void RenderPanels();

	int GetCurrentPanel();

	CUIPanel* GetPanel(int panelID);

private:
	int _CurrentPanel;

	std::map<int, CUIPanel*> panels;
};

#endif