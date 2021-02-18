#include "Base_include.h"
#include "CLevelClient.h"
#include "../NET.h"
#include <iostream>
#include <chrono>
#include <thread>

CLevelClient::CLevelClient()
{
	LF = new LevelFileClient();
}

CLevelClient::CLevelClient(char* LevelName)
{
	LF = new LevelFileClient(LevelName);

	SteamNetworkingIPAddr addrServer; addrServer.Clear();

	char* buff = (char*)malloc(25 * sizeof(buff));
	buff[0] = '\0';

	strcpy(buff, "127.0.0.1:");
	char* itoaBuff = new char;
	strcat(buff, _itoa(STANDART_PORT, itoaBuff, 10));

	addrServer.ParseString(buff);

	client = new Client();
	//client->Run(addrServer);
	ClientThread = std::thread(&Client::Run, client, addrServer);
}

CLevelClient::CLevelClient(char* LevelName, const char* IP4port)
{
	LF = new LevelFileClient(LevelName);

	SteamNetworkingIPAddr addrServer; addrServer.Clear();

	addrServer.ParseString(IP4port);

	client = new Client();
	//client->Run(addrServer);
	ClientThread = std::thread(&Client::Run, client, std::cref(addrServer));
}


CLevelClient::~CLevelClient()
{

}

void CLevelClient::Close()
{
	if (client)
	{
		client->Stop();
		if (ClientThread.joinable())
			ClientThread.join();
	}
}