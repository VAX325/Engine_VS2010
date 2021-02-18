#include <Base_include.h>
#include "../NET.h"

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include "Server.h"

#pragma warning( disable : 26495 6031 )

#pragma comment(lib,"ws2_32.lib")

FileSystem* ServerFs;

CLogManager LogManagerServer;

CScriptSystem* ScriptSystemServer;

lua_State* LSv;

Server server;
std::thread ServerThread;

bool Work = true;

void ServerWindows()
{
    LogManagerServer = CLogManager();
    LogManagerServer.Init(true);

    SteamGameSocketsInit();

    Server server = Server();
    //server.Run(STANDART_PORT);
    ServerThread = std::thread(&Server::Run, &server, STANDART_PORT);

    char* command = new char;

    while(Work)
    {
        cin >> command;

        if (strncmp(command, "/stop", 5) == 0)
        {
            Work = false;
        }
        else if (strncmp(command, "/list", 5) == 0)
        {
            printf("WIP \n");
        }
        else
        {
            printf("There is no command '%s' !\n", command);
        }
    }

    ServerStop();

    SteamGameSocketsDeInit();
}

void ServerInClient()
{
    server = Server();
    server.Run(STANDART_PORT);

    ServerStop();
}

void ServerStop()
{
    server.Stop();
    if (ServerThread.joinable())
        ServerThread.join();
}

CLogManager* GetLogObj()
{
	return &LogManagerServer;
}

FileSystem* GetFileSystemObj()
{
	return ServerFs;
}

CScriptSystem* GetScriptSystemObj()
{
	return ScriptSystemServer;
}

void CallLuaFuncSv(char* LuaFunc)
{
    ScriptSystemServer->CallLuaFunc(LSv, LuaFunc);
}