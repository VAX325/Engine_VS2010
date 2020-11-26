#include "Raw.h"
#include "FileSystem.h"
#include "Main.h"
#include "Utils.h"
#include <Windows.h>

char* OpenRawFile(HWND hWndOwner)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    char szFile[MAX_PATH];
    ZeroMemory(szFile, MAX_PATH);

    strcpy(szFile, (char*)"Untitled1");

    // Инициализируем OPENFILENAME
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWndOwner;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Raw Level (*.rawlvl)\0*.rawlvl\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrTitle = "Save file as...";
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    char* fileName = (char*)malloc(260);

    // отображаем окно Save dialog
    if (GetOpenFileName(&ofn))
    {
        strcpy(fileName, ofn.lpstrFile);
        if (!FindWord(fileName, (char*)".rawlvl"))
        {
            strcat(fileName, ".rawlvl");
        }

        return fileName;
    }

    return (char*)"";
}

void SaveFile(char* FileName, char* data)
{
    FileSystem* fs = GetFileSystemObjCl();

    fs->WriteInFile(data, FileName);
}

char* SaveFileAs(HWND hWndOwner, char* data)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    char szFile[MAX_PATH];
    ZeroMemory(szFile, MAX_PATH);

    strcpy(szFile, (char*)"Untitled1");

    // Инициализируем OPENFILENAME
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWndOwner;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Raw Level (*.rawlvl)\0*.rawlvl\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrTitle = "Save file as...";
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    char* fileName = (char*)malloc(260);

    // отображаем окно Save dialog
    if (GetSaveFileName(&ofn))
    {
        strcpy(fileName, ofn.lpstrFile);
        if(!FindWord(fileName, (char*)".rawlvl"))
        {
            strcat(fileName, ".rawlvl");
        }

        FileSystem* fs = GetFileSystemObjCl();

        fs->WriteInFile(data, fileName);

        return fileName;
    }

    return (char*)"";
}