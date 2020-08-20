#include "FileSystem.h"
#include "Utils.h"
#include <windows.h>

FileSystem::FileSystem()
{
}

FileSystem::FileSystem(const FileSystem& other)
{
}

FileSystem::~FileSystem()
{
}

void FileSystem::WriteInFile(char* data, char* file)
{
	ofstream in = ofstream(file);

	in << data;

	in.close();
}

char* FileSystem::ReadFromFile(char* file)
{
	if (file == NULL)
	{
		return NULL;
	}

	ifstream out = ifstream(file, ios_base::in);

	char buff[512];

	//out.read(buff, out.tellg());
	out.getline(buff, 50);

	out.close();

	return buff;
}

Files FileSystem::GetAllFilesInFolder(char* folder, char* extension)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	char* FilePath = (char*)malloc(500);
	strcpy(FilePath, "*.");

	char* Folder = (char*)malloc(500);

	strcpy(Folder, folder);

	strcat(Folder, FilePath);

	//strcat(FilePath, extension);

	char* FilePathPlusExt = (char*)malloc(500);

	strcpy(FilePathPlusExt, Folder);
	strcat(FilePathPlusExt, extension);

	hf = FindFirstFile(FilePathPlusExt, &FindFileData);

	if (hf != INVALID_HANDLE_VALUE) {
		int i = 0;

		do {
			//files.insert( make_pair(i, (CHAR)FindFileData.cFileName) );
			files[i] = (char*)malloc(512);
			strcpy(files[i], (char*)FindFileData.cFileName);
			i++;
		} while (FindNextFile(hf, &FindFileData) != 0);

		FindClose(hf);
	}

	return files;
}

int FileSystem::GetCountOfFilesInFolder(char* folder, char* extension)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	char* FilePath = (char*)malloc(500);
	strcpy(FilePath, "*.");

	char* Folder = (char*)malloc(500);

	strcpy(Folder, folder);

	strcat(Folder, FilePath);

	char* FilePathPlusExt = (char*)malloc(500);

	strcpy(FilePathPlusExt, Folder);
	strcat(FilePathPlusExt, extension);

	hf = FindFirstFile(FilePathPlusExt, &FindFileData);

	int i = 0;

	if (hf != INVALID_HANDLE_VALUE) {
		do {
			i++;
		} while (FindNextFile(hf, &FindFileData) != 0);

		FindClose(hf);
	}

	return i;
}