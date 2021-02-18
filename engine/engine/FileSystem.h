#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <windows.h>

using namespace std;

typedef map<int, char*> Files;

class FileSystem
{
private:

public:
	FileSystem();

	FileSystem(const FileSystem& other);

	~FileSystem();

	void WriteInFile(char* data, char* file);

	char* ReadFromFile(char* file);

	Files GetAllFilesInFolder(char* folder, char* extension);

	int GetCountOfFilesInFolder(char* folder, char* extension);
};