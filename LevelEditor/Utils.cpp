#include "Utils.h"

bool FindWord(const char * string, const char * word)
{
	const char* p;
	if ((p = strstr(string, word)) == NULL)
		return false;
	else
		return true;
}