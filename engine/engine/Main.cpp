#include <Base_include.h>
#include <Windows.h>
#include "Main.h"
#include "Client/Client.h"
#include "Server/Server.h"

#include "Utils.h"

bool Client;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	int nArgs;
	CommandLineToArgvW(GetCommandLineW(), &nArgs);

	if( nArgs > 1 )
	{
		if (FindWord((char*)lpCmdLine, (char*)"-client"))
		{
			Client = true;
			return ClientMain(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
		}
		else if(FindWord((char*)lpCmdLine, (char*)"-server"))
		{
			Client = false;
			return ServerWindows();
		}
	}

	Client = true;
	return ClientMain(hInstance, hPrevInstance, lpCmdLine, iCmdShow);

	return 13;
}

bool IsClient() 
{
	return Client;
};