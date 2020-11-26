#include <Base_include.h>
#include "NET.h"

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

#include "Main.h"
#include "Client/ClientPerems.h"
#include "Server/ServerPerems.h"
#include <iostream>

//-----------------------------------------ADDRESS-------------------------------------------

Address::Address()
{

}

Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
{
    aA = a;
    bA = b;
    cA = c;
    dA = d;

#pragma message(Reminder "REWORK ADDRESS CONSTRUCTOR!")

    addressA = (a << 24) | (b << 16) | (c << 8) | d;
    portA = port;
}

Address::Address(unsigned int address, unsigned short port)
{
    addressA = address;
    portA = port;
}

unsigned int Address::GetAddress() const
{
    return addressA;
}

unsigned char Address::GetA() const
{
    return aA;
}

unsigned char Address::GetB() const
{
    return bA;
}

unsigned char Address::GetC() const
{
    return cA;
}

unsigned char Address::GetD() const
{
    return dA;
}

unsigned short Address::GetPort() const
{
    return portA;
}

bool Address::operator == (const Address& other) const
{
    if (addressA == other.addressA)
    {
        return true;
    }

    if (portA == other.portA)
    {
        return true;
    }

    return false;
}

bool Address::operator != (const Address& other) const
{
    if(addressA != other.addressA)
    {
        return true;
    }
    
    if(portA != other.portA)
    {
        return true;
    }

    return false;
}

//------------------------------------------SOCKET--------------------------------------------

inline bool InitializeSockets()
{
#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
    return true;
#endif
}

inline void ShutdownSockets()
{
#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
#endif
}

Socket::Socket()
{
}

Socket::~Socket()
{
}

bool Socket::Open(unsigned short port)
{
    if(!InitializeSockets())
    {
        printf("failed to init winsock\n");
        if (IsClient())
        {
            GetLogObjCl()->LogError((char*)"failed to init winsock", false);
            return false;
        }
        else
        {
            GetLogObj()->LogError((char*)"failed to init winsock", false);
            return false;
        }
    }

    handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(handle <= 0)
    {
        if (IsClient())
        {
            GetLogObjCl()->LogError((char*)"failed to create socket", false);
        }
        else
        {
            GetLogObj()->LogError((char*)"failed to create socket", false);
        }
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((unsigned short)port);

    if (::bind(handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
    {
        if (IsClient())
        {
            GetLogObjCl()->LogError((char*)"failed to bind socket", false);
            return false;
        }
        else
        {
            GetLogObj()->LogError((char*)"failed to bind socket", false);
            return false;
        }
    }

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

    int nonBlocking = 1;
    if (fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
    {
        printf("failed to set non-blocking socket\n");
        if (IsClient())
        {
            GetLogObjCl()->LogError((char*)"failed to set non-blocking socket", false);
}
        else
        {
            GetLogObj()->LogError((char*)"failed to set non-blocking socket", false);
        }
        return false;
    }

#elif PLATFORM == PLATFORM_WINDOWS

    DWORD nonBlocking = 1;
    if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
    {
        if (IsClient())
        {
            GetLogObjCl()->LogError((char*)"failed to set non-blocking socket", false);
        }
        else
        {
            GetLogObj()->LogError((char*)"failed to set non-blocking socket", false);
        }
        return false;
    }

#endif

    isOpen = true;

    return true;
}

bool Socket::IsOpen() const
{
    return true;
}

void Socket::Close()
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    close(handle);
#elif PLATFORM == PLATFORM_WINDOWS
    closesocket(handle);
#endif
}

bool Socket::Send(const Address& destination, const void* data, int size)
{
    int sent_bytes = sendto(handle, (const char*)data, size,
        0, (sockaddr*)&destination, sizeof(sockaddr_in));

    if (sent_bytes != (int)data)
    {
        printf("failed to send packet: return value = %d\n", sent_bytes);
        if (IsClient())
        {
            GetLogObjCl()->LogError((char*)"failed to send packet", false);
            return false;
        }
        else
        {
            GetLogObj()->LogError((char*)"failed to send packet", false);
            return false;
        }
        return false;
    }
}

int Socket::Receive(Address& sender, void* data, int size)
{
#if PLATFORM == PLATFORM_WINDOWS
    typedef int socklen_t;
#endif

    sockaddr_in from;
    socklen_t fromLength = sizeof(from);

    int received_bytes = recvfrom(handle, (char*)data, size, 0, (sockaddr*)&from, &fromLength);

    sender = Address(from.sin_addr.s_addr, from.sin_port);

    return received_bytes;
}
