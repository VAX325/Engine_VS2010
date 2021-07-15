#pragma once
#ifndef XMLPARSER_H
#define XMLPARSER_H

#pragma comment(lib, "pugixml.lib")

#include <vector>
#include <array>

class ParsedData;

class CXMLParser
{
public:
	CXMLParser();
	virtual ~CXMLParser();

	std::array<ParsedData*, 15>* ParseXML(const char* file, const char* root);

private:

};

class ParsedData
{
public:
	friend CXMLParser;

	std::array<const char*, 2> operator[](size_t index);
	
	const char* Name();
	int size();

private:
	const char* name;
	std::vector<const char*> key;
	std::vector<const char*> values;

	size_t _size;
};

#endif