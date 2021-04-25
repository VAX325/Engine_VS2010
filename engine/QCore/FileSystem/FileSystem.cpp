#include <Base_include.h>
#include <windows.h>

#include "FileSystem.h"

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

	out.seekg(0, out.end);
	int length = (int)out.tellg();
	out.seekg(0, out.beg);

	char* buff = (char*)malloc(length);

	//out.read(buff, out.tellg());
	out.read(buff, length);

	out.close();

	return buff;
}

Files FileSystem::GetAllFilesInFolder(char* folder, char* extension)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	
	Files files;

	std::string Folder = folder;

	Folder += "*.";

	//strcat(FilePath, extension);

	std::string FilePathPlusExt = Folder.c_str();
	FilePathPlusExt += extension;

	hf = FindFirstFile(FilePathPlusExt.c_str(), &FindFileData);

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
	else
	{
		GetLogManager()->LogError("INVALID_HANDLE_VALUE", true);
	}

	return files;
}

int FileSystem::GetCountOfFilesInFolder(char* folder, char* extension)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	Files files;

	std::string Folder = folder;

	Folder += "*.";

	//strcat(FilePath, extension);

	std::string FilePathPlusExt = Folder.c_str();
	FilePathPlusExt += extension;

	hf = FindFirstFile(FilePathPlusExt.c_str(), &FindFileData);

	int i = 0;

	if (hf != INVALID_HANDLE_VALUE) {
		do {
			i++;
		} while (FindNextFile(hf, &FindFileData) != 0);

		FindClose(hf);
	}

	return i;
}

FileSystem* fs;

void InitFileSystem()
{
	fs = new FileSystem();
}

QCORE_PUBLIC FileSystem* GetFileSystemEx()
{
	return fs;
}
