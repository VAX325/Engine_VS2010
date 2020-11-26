#include <Base_include.h>
#include "Utils.h"

bool FindWord(char* string, char* word)
{
	char* p;
	if ((p = strstr(string, word)) == NULL)
		return false;
	else
		return true;
}