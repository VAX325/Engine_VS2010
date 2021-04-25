#include <Base_include.h>

#include "xmlparser.h"
#include <pugixml/pugixml.hpp>
#include <Windows.h>

XMLParser::XMLParser()
{
}

XMLParser::~XMLParser()
{
}

//#define PredicateFind() struct predicate{char* Find; bool operator()(pugi::xml_attribute attr) const{return strcmp(attr.name(), Find) == 0;}bool operator()(pugi::xml_node node) const{return node.attribute(Find).as_string();} bool operator=(char* Find){}};

pugi::xml_attribute FindAttribute(pugi::xml_node in, char* attribute)
{
    for (pugi::xml_attribute attrib = in.first_attribute(); attrib; attrib = attrib.next_attribute())
        if (strcmp(attrib.name(), attribute) == 0)
            return attrib;
    return pugi::xml_attribute();
}

pugi::xml_attribute FindAttribute(pugi::xml_node_iterator in, char* attribute)
{
    for (pugi::xml_attribute attrib = in->first_attribute(); attrib; attrib = attrib.next_attribute())
        if (strcmp(attrib.name(), attribute) == 0)
            return attrib;
    return pugi::xml_attribute();
}

void XMLParser::ParseXML(char* file, char* root, KeysAtrributesNValues* prs)
{
    //KeysAtrributesNValues prs;

    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(file);

    pugi::xml_node root1 = doc.child(root);

    pugi::xml_object_range<pugi::xml_node_iterator> children = root1.children();
    
    pugi::xml_node_iterator it = children.begin();

    while(it != children.end() && it->parent() != NULL)
    {
        char* NameBuff = (char*)malloc(128);

        strcpy(NameBuff, (char*)it->name());
        
        char* Attributes[10];
        char* AttributesValues[10];

        ZeroMemory(Attributes, sizeof(Attributes));
        ZeroMemory(AttributesValues, sizeof(AttributesValues));

        /*strcpy(Attributes[0], attribName);*/ 
        Attributes[0] = _strdup((char*)it->first_attribute().name());
        /*strcpy(AttributesValues[0], (char*)it->first_attribute().value());*/
        AttributesValues[0] = _strdup((char*)it->first_attribute().value());

        int i = 0;
        
        while (FindAttribute(it, Attributes[i]).next_attribute())
        {
            Attributes[i+1] = _strdup((char*)FindAttribute(it, Attributes[i]).next_attribute().name());
            
            AttributesValues[i+1] = _strdup((char*)FindAttribute(it, Attributes[i]).next_attribute().value());

            //strcat(Attributes[i + 1], (char*)FindAttribute(it, Attributes[i]).next_attribute().name());

            //strcat(AttributesValues[i + 1], (char*)FindAttribute(it, Attributes[i]).next_attribute().value());

            i++;
        }

        arr arr1;
        arr arr2;

        std::swap(arr1.chr, Attributes);
        std::swap(arr2.chr, AttributesValues);

        //for(int j = 0; Attributes[j] != NULL; j++)
        //{
        
        Vector2<arr, arr> f = { arr1, arr2 };

        prs->insert(std::make_pair(NameBuff, f));
        //prs[NameBuff] = { arr1, arr2 };
        //}

        it++;
    }

    //std::swap(&pars, &prs);
    //return &prs;
}