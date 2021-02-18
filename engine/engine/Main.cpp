#include <Base_include.h>
#include <Windows.h>
#include "Main.h"
#include "Client/Client.h"
#include "Server/Server.h"

#include "Utils.h"

bool Client;

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
int main(int argc, char* argv[])
{
	//int nArgs;
	//CommandLineToArgvW(GetCommandLineW(), &nArgs);

	if( argc > 1 )
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
			}
		}
	}
	else
	{
		Client = true;
		return ClientMain(argc, argv);
	}

	return 0;
}

bool IsClient() 
{
	return Client;
};