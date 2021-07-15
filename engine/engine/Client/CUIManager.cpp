#include <Base_include.h>

#include "CUIManager.h"
#include "../xmlparser.h"
#include "../Utils.h"
#include "ClientPerems.h"

#include <sstream>
#include <iostream>

CUIManager::CUIManager()
{
	
}

CUIManager::~CUIManager()
{

}

inline CUIText* TextSearch(ParsedData* prs)
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int color = 0xffffffff;

	const char* text = "null";
	const char* font = "Arial";

	for(int i = 0; i != prs->size(); i++)
	{
		const char* key = (*prs)[i][0];
		const char* value = (*prs)[i][1];

		if(strcmp(key, "x") == 0)
		{
			x = atoi(value);
		}
		else if(strcmp(key, "y") == 0)
		{
			y = atoi(value);
		}
		else if (strcmp(key, "w") == 0)
		{
			w = atoi(value);
		}
		else if (strcmp(key, "h") == 0)
		{
			h = atoi(value);
		}
		else if (strcmp(key, "color") == 0)
		{
			color = atoi(value);
		}
		else if (strcmp(key, "value") == 0)
		{
			text = value;
		}
		else if (strcmp(key, "font") == 0)
		{
			font = value;
		}
	}

	CUIText* Text = new CUIText(x, y, w, h, text);
	Text->SetFont(font);
	Text->SetColor(color);
	Text->SetLayer(6);

	return Text;
}

inline CUIImage* ImageSearch(ParsedData* prs)
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	std::string name = "../gamedata/sprites/UI/";
	
	for (int i = 0; i != prs->size(); i++)
	{
		const char* key = (*prs)[i][0];
		const char* value = (*prs)[i][1];

		if (strcmp(key, "x") == 0)
		{
			x = atoi(value);
		}
		else if (strcmp(key, "y") == 0)
		{
			y = atoi(value);
		}
		else if (strcmp(key, "w") == 0)
		{
			w = atoi(value);
		}
		else if (strcmp(key, "h") == 0)
		{
			h = atoi(value);
		}
		else if (strcmp(key, "name") == 0)
		{
			name = name + value;
			name = name + ".png";
		}
	}

	CUIImage* img = new CUIImage(x, y, w, h, name.c_str());

	return img;
}

inline CUIButton* ButtonSearch(ParsedData* prs)
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	std::string name = "null";
	std::string sprite = "../gamedata/sprites/UI/";
	std::string text = "";

	LuaFuncPtr* funct_ptr = nullptr;

	for (int i = 0; i != prs->size(); i++)
	{
		const char* key = (*prs)[i][0];
		const char* value = (*prs)[i][1];

		if (strcmp(key, "x") == 0)
		{
			x = atoi(value);
		}
		else if (strcmp(key, "y") == 0)
		{
			y = atoi(value);
		}
		else if (strcmp(key, "w") == 0)
		{
			w = atoi(value);
		}
		else if (strcmp(key, "h") == 0)
		{
			h = atoi(value);
		}
		else if (strcmp(key, "text") == 0)
		{
			text = value;
		}
		else if (strcmp(key, "name") == 0) 
		{
			sprite = sprite + value;
			sprite = sprite + ".png";
		}
		else if (strcmp(key, "sprite") == 0)
		{
			name = value;
		}
		else if (strcmp(key, "OnClick") == 0)
		{
			funct_ptr = GetLuaFuncPtrCl(value);
		}
	}

	CUIButton* btn = new CUIButton(x, y, w, h, sprite.c_str(), text, &funct_ptr);
	btn->SetLayer(5);

	return btn;
}

void CUIManager::LoadPanels()
{
	_CurrentPanel = UI_NO_PANEL;

	Files configs = GetFileSystemEx()->GetAllFilesInFolder((char*)"../gamedata/configs/ui/", (char*)"xml");
	CXMLParser* xml_parser = GetParser();

	int j = 0;
	for (auto i = configs.begin(); i != configs.end(); i++)
	{
		std::string path = "../gamedata/configs/ui/";
		path = path + i->second;

		std::array<ParsedData*, 15>* prs = xml_parser->ParseXML(path.c_str(), "panel");

		panels[j] = new CUIPanel(i->second);

		for(int i = 0; i != prs->size(); i++)
		{
			if((*prs)[i] == nullptr)
			{
				break;
			}

			if (FindWord((*prs)[i]->Name(), "Text"))
			{
				CUIText* text = TextSearch((*prs)[i]);
				panels[j]->AddElement(text);
			}
			else if (FindWord((*prs)[i]->Name(), "Image"))
			{
				CUIImage* img = ImageSearch((*prs)[i]);
				panels[j]->AddElement(img);
			}
			else if (FindWord((*prs)[i]->Name(), "Button"))
			{
				CUIButton* btn = ButtonSearch((*prs)[i]);
				panels[j]->AddElement(btn);
			}
		}
		j++;
	}
}

void CUIManager::RenderPanels()
{
	for(auto it = panels.begin(); it != panels.end(); it++)
	{
		
	}
}

int CUIManager::GetCurrentPanel()
{
	return _CurrentPanel;
}

CUIPanel* CUIManager::GetPanel(int panelID)
{
	return panels[panelID];
}

void CUIManager::ShowPanel(int panelID)
{
	if (_CurrentPanel != UI_NO_PANEL)
	{
		panels[_CurrentPanel]->HidePanel();
	}
	panels[panelID]->ShowPanel();
	_CurrentPanel = panelID;
}

void CUIManager::HidePanel(int panelID)
{
	if (_CurrentPanel != UI_NO_PANEL)
	{
		panels[_CurrentPanel]->HidePanel();
	}
	panels[panelID]->HidePanel();
	_CurrentPanel = UI_NO_PANEL;
}

void CUIManager::ShowPanel(const char* panelName)
{
	for (int i = 0; i != GetCountOfPanels(); i++)
	{
		if (strcmp(GetPanelName(i), panelName) == 0)
		{
			ShowPanel(i);
			break;
		}
	}
}

void CUIManager::HidePanel(const char* panelName)
{
	for (int i = 0; i != GetCountOfPanels(); i++)
	{
		if (strcmp(GetPanelName(i), panelName) == 0)
		{
			HidePanel(i);
			break;
		}
	}
}

int CUIManager::GetCountOfPanels()
{
	return panels.size();
}

char* CUIManager::GetPanelName(int panelId)
{
	return panels[panelId]->GetName();
}
