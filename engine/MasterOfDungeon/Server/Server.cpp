#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "Server.h"

#define STANDART_PORT 27063

#pragma comment(lib,"ws2_32.lib")

int ServerWindows()
{
	CLogManager LogManager = CLogManager();

	AllocConsole();
	freopen("CONOUT$", "w+", stdout);
	freopen("CONIN$", "w+", stdin);

	WSADATA wsaData;
	SOCKET SendRecvSocket;

	sockaddr_in ServerAddr, ClientAddr;
	
	int err, maxlen = 512, ClientAddrSize = sizeof(ClientAddr);
	
	char* recvbuf = new char[maxlen];
	char* result_string = new char[maxlen];

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(STANDART_PORT);
	
	err = bind(SendRecvSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
	
	if (err == SOCKET_ERROR) 
	{
		printf("bind failed: %d\n", WSAGetLastError());
		closesocket(SendRecvSocket);
		WSACleanup();

		char buff[100];

		char* countc = _itoa(WSAGetLastError(), buff, 10);

		string wserrror = "SOCKET_ERROR: ";

		wserrror = wserrror + countc;

		LogManager.LogError((char*)wserrror.c_str(), false);

		return 1;
	}

	while (true) 
	{
		err = recvfrom(SendRecvSocket, recvbuf, maxlen, 0, (sockaddr*)&ClientAddr, &ClientAddrSize);
		LogManager.LogMsg("Wait for mesages");
		if (err > 0) {
			
			recvbuf[err] = 0;

			result_string = recvbuf;

			sendto(SendRecvSocket, result_string, strlen(result_string), 0, (sockaddr*)&ClientAddr, sizeof(ClientAddr));

			printf("Sent answer: %s\n", result_string);

			LogManager.LogMsg("Send success!");
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(SendRecvSocket);
			WSACleanup();

			char buff[100];

			char* countc = _itoa(WSAGetLastError(), buff, 10);

			string wserrror = "RECIVE_FAILED: ";

			wserrror = wserrror + countc;

			LogManager.LogError((char*)wserrror.c_str(), false);

			return 1;
		}
	}

	return 0;
}

char* ServerInClient()
{
	WSADATA wsaData;
	SOCKET SendRecvSocket;

	sockaddr_in ServerAddr, ClientAddr;

	int err, maxlen = 512, ClientAddrSize = sizeof(ClientAddr);

	char* recvbuf = new char[maxlen];
	char* result_string = new char[maxlen];

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(STANDART_PORT);

	err = bind(SendRecvSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));

	if (err == SOCKET_ERROR)
	{
		closesocket(SendRecvSocket);
		WSACleanup();
		return "Fuck! We scred up! SOCKET_ERROR";
	}

	err = recvfrom(SendRecvSocket, recvbuf, maxlen, 0, (sockaddr*)&ClientAddr, &ClientAddrSize);
	if (err > 0) {

		recvbuf[err] = 0;

		result_string = recvbuf;

		sendto(SendRecvSocket, result_string, strlen(result_string), 0, (sockaddr*)&ClientAddr, sizeof(ClientAddr));

		return "All ok! Result sended!";
	}
	else
	{
		closesocket(SendRecvSocket);
		WSACleanup();
		return "Fuck! We scred up! Something with reciving!";
	}
}