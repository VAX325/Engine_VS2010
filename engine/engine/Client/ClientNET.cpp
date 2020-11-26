#include <Base_include.h>
#include "ClientNET.h"

#include "ClientPerems.h"

#include <stdlib.h>
#include <stdio.h>

#define STANDART_PORT 27063

ClientNET::ClientNET(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
{
    serverAddress = Address(a, b, c, d, port);

    if (!socket.Open(STANDART_PORT))
    {
        GetLogObjCl()->LogError((char*)"failed to create socket!", true);
    }
}

ClientNET::ClientNET(unsigned int address, unsigned short port)
{
    serverAddress = Address(address, port);

    if (!socket.Open(STANDART_PORT))
    {
        GetLogObjCl()->LogError((char*)"failed to create socket!", true);
    }
}

ClientNET::~ClientNET()
{
}

char* ClientNET::Send(const char data[])
{
    if (!socket.Send(serverAddress, data, sizeof(data))) 
    {
        return (char*)"Socket send error!";
    }

    return (char*)"OK";
}

int ClientNET::Recive()
{
    unsigned char buffer[256];
    return socket.Receive(serverAddress, buffer, sizeof(buffer));
}