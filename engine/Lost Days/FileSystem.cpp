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
	ifstream out = ifstream(file);

	char* buff;

	out.read(buff, out.tellg());

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
			files[i] = (CHAR)FindFileData.cFileName;
			i++;

		} while (FindNextFile(hf, &FindFileData) != 0);
		
		FindClose(hf);
	}

	return files;
}