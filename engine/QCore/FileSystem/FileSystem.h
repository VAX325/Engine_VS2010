#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <fstream>
#include <iostream>
#include <map>
#include <windows.h>

#include "../../UsefullMacro.h"

using namespace std;

typedef map<int, char*> Files;

class QCORE_PUBLIC FileSystem
{
private:

public:
	FileSystem();

	FileSystem(const FileSystem& other);

	~FileSystem();

	void WriteInFile(char* data, char* file);

	const char* ReadFromFile(const char* file);

	Files GetAllFilesInFolder(char* folder, char* extension);

	int GetCountOfFilesInFolder(char* folder, char* extension);
};

void InitFileSystem();

QCORE_PUBLIC FileSystem* GetFileSystemEx();

#endif