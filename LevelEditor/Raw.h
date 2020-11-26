#pragma once
#include <Windows.h>

char* OpenRawFile(HWND hWndOwner);
void SaveFile(char* FileName, char* data);
char* SaveFileAs(HWND hWndOwner, char* data);