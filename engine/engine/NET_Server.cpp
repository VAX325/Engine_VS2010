#include <Base_include.h>
#include "NET.h"

#include "CLogManager.h"
#include <iostream>
#include <chrono>
#include <thread>

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

bool ImAcrutch = false;

Server::Server()
{
	sv_pCallbackInstance = this;
}

Server::~Server()
{

}

void Server::Run(uint16 nPort)
{
	m_pInterfaceSv = SteamNetworkingSockets();

	// Start listening
	SteamNetworkingIPAddr serverLocalAddr;
	serverLocalAddr.Clear();
	serverLocalAddr.m_port = nPort;
	SteamNetworkingConfigValue_t opt;
	opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);
	m_hListenSock = m_pInterfaceSv->CreateListenSocketIP(serverLocalAddr, 1, &opt);
	if (m_hListenSock == k_HSteamListenSocket_Invalid)
		GetLogManager()->LogError("Failed to listen on port %d", true, nPort);
	m_hPollGroup = m_pInterfaceSv->CreatePollGroup();
	if (m_hPollGroup == k_HSteamNetPollGroup_Invalid)
		GetLogManager()->LogError("Failed to listen on port %d", true, nPort);
	GetLogManager()->LogMsg("Server listening on port %d\n", nPort);

	while (!ImAcrutch)
	{
		PollConnectionStateChanges();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	// Close all the connections
	GetLogManager()->LogMsg("Closing connections...\n");
	for (auto it : m_mapClients)
	{
		SendStringToClient(it.first, "Server is shutting down.  Goodbye.");

		m_pInterfaceSv->CloseConnection(it.first, 0, "Server Shutdown", true);
	}
	m_mapClients.clear();

	m_pInterfaceSv->CloseListenSocket(m_hListenSock);
	m_hListenSock = k_HSteamListenSocket_Invalid;

	m_pInterfaceSv->DestroyPollGroup(m_hPollGroup);
	m_hPollGroup = k_HSteamNetPollGroup_Invalid;
}

void Server::Stop()
{
	ImAcrutch = true;
}

void Server::SendStringToClient(HSteamNetConnection conn, const char* str)
{
	m_pInterfaceSv->SendMessageToConnection(conn, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr);
}

void Server::SendStringToAllClients(const char* str, HSteamNetConnection except)
{
	for (auto& c : m_mapClients)
	{
		if (c.first != except)
			SendStringToClient(c.first, str);
	}
}

void Server::SendDataToClient(HSteamNetConnection conn, void* data)
{
	//m_pInterface->SendMessageToConnection(conn, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr);
	m_pInterfaceSv->SendMessageToConnection(conn, data, (uint32)sizeof(data), k_nSteamNetworkingSend_Reliable, nullptr);
}

void Server::SendDataToAllClients(void* data, HSteamNetConnection except)
{
	for (auto& c : m_mapClients)
	{
		if (c.first != except)
			SendDataToClient(c.first, data);
	}
}

void Server::SetClientNick(HSteamNetConnection hConn, const char* nick)
{
	m_mapClients[hConn].m_sNick = nick;

	m_pInterfaceSv->SetConnectionName(hConn, nick);
}

void Server::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	char temp[1024];

	switch (pInfo->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_None:
		break;

	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
	{
		if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
		{

			auto itClient = m_mapClients.find(pInfo->m_hConn);
			assert(itClient != m_mapClients.end());

			const char* pszDebugLogAction;
			if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
			{
				pszDebugLogAction = "problem detected locally";
				sprintf(temp, "Alas, %s hath fallen into shadow.  (%s)", itClient->second.m_sNick.c_str(), pInfo->m_info.m_szEndDebug);
			}
			else
			{
				pszDebugLogAction = "closed by peer";
				sprintf(temp, "%s hath departed", itClient->second.m_sNick.c_str());
			}

			GetLogManager()->LogMsg("Connection %s %s, reason %d: %s\n",
				pInfo->m_info.m_szConnectionDescription,
				pszDebugLogAction,
				pInfo->m_info.m_eEndReason,
				pInfo->m_info.m_szEndDebug
			);

			m_mapClients.erase(itClient);

			SendStringToAllClients(temp);
		}
		else
		{
			assert(pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
		}

		m_pInterfaceSv->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		break;
	}

	case k_ESteamNetworkingConnectionState_Connecting:
	{
		assert(m_mapClients.find(pInfo->m_hConn) == m_mapClients.end());

		GetLogManager()->LogMsg("Connection request from %s", pInfo->m_info.m_szConnectionDescription);

		if (m_pInterfaceSv->AcceptConnection(pInfo->m_hConn) != k_EResultOK)
		{
			m_pInterfaceSv->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
			GetLogManager()->LogMsg("Can't accept connection.  (It was already closed?)");
			break;
		}

		if (!m_pInterfaceSv->SetConnectionPollGroup(pInfo->m_hConn, m_hPollGroup))
		{
			m_pInterfaceSv->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
			GetLogManager()->LogMsg("Failed to set poll group?");
			break;
		}

		char nick[64];
		sprintf(nick, "BraveWarrior%d", 10000 + (rand() % 100000));

		if (m_mapClients.empty())
		{
			SendStringToClient(pInfo->m_hConn, "You're alone on server");
		}
		else
		{
			sprintf(temp, "There are %d players on server", (int)m_mapClients.size());
			for (auto& c : m_mapClients)
				SendStringToClient(pInfo->m_hConn, c.second.m_sNick.c_str());
		}

		m_mapClients[pInfo->m_hConn];
		SetClientNick(pInfo->m_hConn, nick);
		break;
	}

	case k_ESteamNetworkingConnectionState_Connected:
		break;

	default:
		break;
	}
}

void Server::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	sv_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
}

void Server::PollConnectionStateChanges()
{
	sv_pCallbackInstance = this;
	m_pInterfaceSv->RunCallbacks();
}

Server* Server::sv_pCallbackInstance = nullptr;