#pragma once
#include <map>
#include "NVector.h"

#pragma comment(lib, "pugixml.lib")

//Uber костыль
struct arr
{
	char* chr[10];
};

//using arr = std::array<char*, 10>;

typedef std::map<char*, Vector2<arr, arr>> KeysAtrributesNValues;

class XMLParser
{
public:
	XMLParser();
	~XMLParser();

	void ParseXML(char* file, char* root, KeysAtrributesNValues* prs);

private:

};