#include <Base_include.h>
#include "../NET.h"

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

#include <stdlib.h>
#include <stdio.h>
#include "Server.h"

#pragma warning( disable : 26495 6031 )

#define STANDART_PORT 27063

#pragma comment(lib,"ws2_32.lib")

FileSystem* ServerFs;

CLogManager LogManagerServer;

CScriptSystem* ScriptSystemServer;

int ServerWindows()
{
	AllocConsole();
	freopen("CONOUT$", "w+", stdout);
	freopen("CONIN$", "w+", stdin);

    Socket socket;
    if (!socket.Open(STANDART_PORT))
    {
        printf("failed to create socket!\n");
        GetLogObj()->LogError((char*)"failed to create socket!", true);
        return false;
    }

    // send a packet

    /*const char data[] = "hello world!";
    socket.Send(Address(127, 0, 0, 1, STANDART_PORT), data, sizeof(data));*/

    // receive packets

    while (true)
    {
        Address sender;
        unsigned char buffer[256];
        int bytes_read = socket.Receive(sender, buffer, sizeof(buffer));
        if (!bytes_read)
            break;
        // process packet
    }

	return 0;
}

char* ServerInClient()
{
    Socket socket;
    if (!socket.Open(STANDART_PORT))
    {
        return (char*)"failed to create socket!";
    }

    // send a packet

    const char data[] = "hello world!";
    socket.Send(Address(127, 0, 0, 1, STANDART_PORT), data, sizeof(data));

    // receive packets

    while (true)
    {
        Address sender;
        unsigned char buffer[256];
        int bytes_read = socket.Receive(sender, buffer, sizeof(buffer));
        if (!bytes_read)
            break;
        // process packet
    }
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