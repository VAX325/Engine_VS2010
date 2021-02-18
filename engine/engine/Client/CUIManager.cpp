#include "Base_include.h"
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

void CUIManager::LoadPanels()
{
	_CurrentPanel = UI_NO_PANEL;

	Files configs = GetFileSystemObjCl()->GetAllFilesInFolder((char*)"../gamedata/configs/ui/", (char*)"xml");

	XMLParser xml_parser;

	auto confit = configs.begin();

	int j = 0;

	while(confit != configs.end())
	{
		KeysAtrributesNValues prs;

		char* buff = (char*)malloc(128);

		strcpy(buff, "../gamedata/configs/ui/");

		strcat(buff, confit->second);

		xml_parser.ParseXML(buff, (char*)"panel", &prs);

		auto it = prs.begin();

		panels[j] = new CUIPanel(confit->second);

		int findedText = 0;
		int findedButtonds = 0;

		while (it != prs.end())
		{
			if (FindWord(it->first, (char*)"CText"))
			{
				int i = 0;

				float x = 0, y = 0;
				float w = 0;
				float h = 0;
				char* Text = (char*)malloc(64);
				char* FontName = (char*)malloc(64);

				D3DCOLOR Color;

				ZeroMemory(Text, sizeof(Text));

				while (it->second.first.chr[i] != NULL)
				{
					if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
					{
						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"x"))
							{
								x = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"y"))
							{
								y = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"w"))
							{
								w = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"h"))
							{
								h = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"value"))
							{
								strcpy(Text, it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"font"))
							{
								strcpy(FontName, it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"color"))
							{
								Color = std::stof(it->second.second.chr[i]);
								i++;
							}
						}
					}
				}
				CUIText* text = new CUIText(x, y, w, h, (std::string)Text);
				text->SetFont(GetD3D9Device(), FontName);
				text->SetColor(Color);
				panels[j]->AddElement(text, Text);
			}

			if (FindWord(it->first, (char*)"CSprite"))
			{
				int i = 0;

				float x = 0, y = 0;
				float w = 0;
				float h = 0;
				char* Name = (char*)malloc(64);

				bool hNeed = false;
				bool wNeed = false;

				ZeroMemory(Name, sizeof(Name));

				while (it->second.first.chr[i] != NULL)
				{
					if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
					{
						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"x"))
							{
								x = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"y"))
							{
								y = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"w"))
							{
								w = std::stof(it->second.second.chr[i]);
								if (w == -1)
								{
									wNeed = true;
								}
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"h"))
							{
								h = std::stof(it->second.second.chr[i]);
								if(h == -1)
								{
									hNeed = true;
								}
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"name"))
							{
								strcpy(Name, it->second.second.chr[i]);
								i++;
							}
						}
					}
				}

				char* texturePath = (char*)malloc(128);
				strcpy(texturePath, "../gamedata/sprites/UI/");
				strcat(texturePath, Name);
				strcat(texturePath, ".png");

				CSprite* sprite = new CSprite(GetD3D9Device(), texturePath, GetWindowW(), GetWindowH(), Name);

				if (sprite == NULL)
				{
					GetLogObjCl()->LogError((char*)"Can't find sprite! Please check your .xml configuration file!", true);
				}

				if (hNeed)
				{
					h = sprite->GetH();
				}
				if (wNeed)
				{
					w = sprite->GetW();
				}

				CUISprite* UISprite = new CUISprite(x, y, w, h, sprite, false);

				panels[j]->AddElement(UISprite, Name);
			}

			if (FindWord(it->first, (char*)"CButton"))
			{
				int i = 0;

				float x = 0, y = 0;
				float w = 0;
				float h = 0;
				char* Name = (char*)malloc(64);
				char* SpriteName = (char*)malloc(64);
				char* OnClickName = (char*)malloc(64);
				char* Text = (char*)malloc(64);

				ZeroMemory(Name, sizeof(Name));
				ZeroMemory(SpriteName, sizeof(SpriteName));
				ZeroMemory(OnClickName, sizeof(OnClickName));
				ZeroMemory(Text, sizeof(Text));

				while (it->second.first.chr[i] != NULL)
				{
					if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
					{
						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"x"))
							{
								x = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"y"))
							{
								y = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"w"))
							{
								w = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"h"))
							{
								h = std::stof(it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"name"))
							{
								strcpy(Name, it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"sprite"))
							{
								strcpy(SpriteName, it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"text"))
							{
								strcpy(Text, it->second.second.chr[i]);
								i++;
							}
						}

						if (it->second.first.chr[i] != NULL && it->second.second.chr[i] != NULL)
						{
							if (FindWord(it->second.first.chr[i], (char*)"OnClick"))
							{
								strcpy(OnClickName, it->second.second.chr[i]);
								i++;
							}
						}
					}
				}
				char* texturePath = (char*)malloc(128);
				strcpy(texturePath, "../gamedata/sprites/UI/");
				strcat(texturePath, SpriteName);
				strcat(texturePath, ".png");
				CSprite* sprite = new CSprite(GetD3D9Device(), texturePath, GetWindowW(), GetWindowH(), SpriteName );

				if (sprite == NULL)
				{
					GetLogObjCl()->LogMsg((char*)"Can't find sprite on button. Is good?");
				}
				LuaFuncPtr* ptr = GetLuaFuncPtrCl(OnClickName);//(LuaFuncPtr*)malloc(sizeof(LuaFuncPtr));
				//memcpy(ptr, GetLuaFuncPtrCl(OnClickName), sizeof(LuaFuncPtr));

				CButton* button = new CButton(x, y, w, h, sprite, Text, &ptr);

				panels[j]->AddElement(button, Name);
			}

			it++;
		}
		confit++;
		j++;
	}
}

void CUIManager::RenderPanels()
{
	for(auto it = panels.begin(); it != panels.end(); it++)
	{
		it->second->Render(GetD3D9Device());
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

int CUIManager::GetCountOfPanels()
{
	return panels.size();
}

char* CUIManager::GetPanelName(int panelId)
{
	return panels[panelId]->GetName();
}
