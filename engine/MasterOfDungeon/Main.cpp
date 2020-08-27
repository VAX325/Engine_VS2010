#include <Windows.h>
#include "Client/Client.h"
#include "Server/Server.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	int nArgs;
	CommandLineToArgvW(GetCommandLineW(), &nArgs);

	if( nArgs > 1 )
	{
		if ( strcmp((char*)lpCmdLine, "-client") == 0)
		{
			return ClientMain(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
		}
		else if( strcmp((char*)lpCmdLine, "-server") == 0)
		{
			return ServerWindows();
		}
	}
	else
	{
		return ClientMain(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
	}
}