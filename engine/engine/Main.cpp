#include <Base_include.h>

#include "Main.h"
#include "Client/Client.h"
#include "Server/Server.h"

#include "Utils.h"

#if IS_WIN32
#include <Windows.h>
#endif

bool Client;

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i = 0; i != argc; i++)
		{
			if (FindWord((char*)argv[i], (char*)"-client"))
			{
				Client = true;
				return ClientMain(argc, argv);
			}
			if (FindWord((char*)argv[i], (char*)"-server"))
			{
				Client = false;
				ServerWindows();
				return 0;
			}
		}
	}
	Client = true;
	return ClientMain(argc, argv);

	return 0;
}

bool IsClient()
{
	return Client;
};