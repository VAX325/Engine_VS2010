#include <Base_include.h>

#include "xmlparser.h"

#include <pugixml/pugixml.hpp>
#include <array>

CXMLParser::CXMLParser()
{
}

CXMLParser::~CXMLParser()
{
}

pugi::xml_attribute FindAttribute(pugi::xml_node in, const char* attribute)
{
    for (pugi::xml_attribute attrib = in.first_attribute(); attrib; attrib = attrib.next_attribute())
        if (strcmp(attrib.name(), attribute) == 0)
            return attrib;
    return pugi::xml_attribute();
}

pugi::xml_attribute FindAttribute(pugi::xml_node_iterator in, const char* attribute)
{
    for (pugi::xml_attribute attrib = in->first_attribute(); attrib; attrib = attrib.next_attribute())
        if (strcmp(attrib.name(), attribute) == 0)
            return attrib;
    return pugi::xml_attribute();
}

std::array<ParsedData*, 15>* CXMLParser::ParseXML(const char* file, const char* root)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file);
    pugi::xml_node root1 = doc.child(root);
    pugi::xml_object_range<pugi::xml_node_iterator> children = root1.children();

    std::array<ParsedData*, 15>* prs_array = new std::array<ParsedData*, 15>();

    int j = 0;
    for(auto it = children.begin(); it != children.end() && it->parent() != NULL; it++)
    {
        ParsedData* prs = new ParsedData();

        prs->name = _strdup(it->name());

        prs->key.push_back(_strdup(it->first_attribute().name()));
        prs->values.push_back(_strdup(it->first_attribute().value()));

        for (int i = 1 /*Because 0 index is exists!*/; FindAttribute(it, prs->key[i-1]).next_attribute(); i++)
        {
            prs->key.push_back(_strdup(FindAttribute(it, prs->key[i - 1]).next_attribute().name()));
            prs->values.push_back(_strdup((char*)FindAttribute(it, prs->key[i - 1]).next_attribute().value()));
        }

        prs->_size = prs->key.size();

        (*prs_array)[j] = prs;
        j++;
    }

    return prs_array;
}

std::array<const char*, 2> ParsedData::operator[](size_t index)
{
    if (index < _size)
    {
        return { key[index], values[index] };
    }
    return {0};
}

const char* ParsedData::Name()
{
    return name;
}

int ParsedData::size()
{
    return _size;
}
