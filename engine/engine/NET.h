#pragma once

#pragma comment(lib, "GameNetworkingSockets.lib")

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

#include <iostream>
#include <string>
#include <map>

//This need to call the static func
void SteamGameSocketsInit();
//This need to call the static func
void SteamGameSocketsDeInit();

class Server
{
public:
	Server();
	~Server();

	void Run(uint16 nPort);

	void Stop();

	void SendStringToClient(HSteamNetConnection conn, const char* str);

	void SendStringToAllClients(const char* str, HSteamNetConnection except = k_HSteamNetConnection_Invalid);

	void SendDataToClient(HSteamNetConnection conn, void* data);

	void SendDataToAllClients(void* data, HSteamNetConnection except = k_HSteamNetConnection_Invalid);

	void SetClientNick(HSteamNetConnection hConn, const char* nick);

private:

	HSteamListenSocket m_hListenSock;
	HSteamNetPollGroup m_hPollGroup;
	ISteamNetworkingSockets* m_pInterface;

	struct Client_t
	{
		std::string m_sNick;
	};

	std::map< HSteamNetConnection, Client_t > m_mapClients;

	void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

	static Server* s_pCallbackInstance;
	static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);

	void PollConnectionStateChanges();
};

class Client
{
public:
	Client();
	~Client();

	void Run(const SteamNetworkingIPAddr& serverAddr);

	void Stop();

private:
	HSteamNetConnection m_hConnection;
	ISteamNetworkingSockets* m_pInterfaceCl;

	void PollIncomingMessages();

	void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

	static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
	
	void PollConnectionStateChanges();
	
	static Client* cl_pCallbackInstance;

	bool g_bQuit;
};