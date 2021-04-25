#include <Base_include.h>

#include "NET.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

#include <cassert>

Client::Client()
{
	g_bQuit = false;

	cl_pCallbackInstance = this;
}

Client::~Client()
{
}

void Client::Run(const SteamNetworkingIPAddr& serverAddr)
{
	m_pInterfaceCl = SteamNetworkingSockets();
	if (m_pInterfaceCl == nullptr)
	{
		GetLogManagerEx()->LogError("Failed to create SteamNetworkingSockets", true);
	}

	char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
	serverAddr.ToString(szAddr, sizeof(szAddr), true);

	GetLogManagerEx()->LogMsg("Connecting to server at %s", szAddr);

	SteamNetworkingConfigValue_t opt;
	opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);

	m_hConnection = m_pInterfaceCl->ConnectByIPAddress(serverAddr, 1, &opt);

	if (m_hConnection == k_HSteamNetConnection_Invalid)
		GetLogManagerEx()->LogError("Failed to create connection", true);

	while (!g_bQuit)
	{
		PollIncomingMessages();
		PollConnectionStateChanges();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void Client::Stop()
{
	g_bQuit = true;
}

void Client::SendString(const char* str)
{
	m_pInterfaceCl->SendMessageToConnection(m_hConnection, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr);
}

void Client::SendData(void* data)
{
	m_pInterfaceCl->SendMessageToConnection(m_hConnection, data, (uint32)sizeof(data), k_nSteamNetworkingSend_Reliable, nullptr);
}

void Client::PollIncomingMessages()
{
	while (!g_bQuit)
	{
		ISteamNetworkingMessage* pIncomingMsg = nullptr;
		int numMsgs = m_pInterfaceCl->ReceiveMessagesOnConnection(m_hConnection, &pIncomingMsg, 1);
		if (numMsgs == 0)
			break;
		if (numMsgs < 0)
			GetLogManagerEx()->LogError("Error checking for messages", true);

		// Just echo anything we get from the server
		fwrite(pIncomingMsg->m_pData, 1, pIncomingMsg->m_cbSize, stdout);
		fputc('\n', stdout);

		// We don't need this anymore.
		pIncomingMsg->Release();

		//m_pInterfaceCl->SendMessageToConnection(m_hConnection, cmd.c_str(), (uint32)cmd.length(), k_nSteamNetworkingSend_Reliable, nullptr);
	}
}

void Client::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	assert(pInfo->m_hConn == m_hConnection || m_hConnection == k_HSteamNetConnection_Invalid);

	switch (pInfo->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_None:
		break;

	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
	{
		g_bQuit = true;

		if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
		{
			GetLogManagerEx()->LogMsg("Can't find remote host.  (%s)", pInfo->m_info.m_szEndDebug);
		}
		else if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
		{
			GetLogManagerEx()->LogMsg("Lost contact with the host.  (%s)", pInfo->m_info.m_szEndDebug);
		}
		else
		{
			GetLogManagerEx()->LogMsg("Disconected.  (%s)", pInfo->m_info.m_szEndDebug);
		}

		m_pInterfaceCl->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		m_hConnection = k_HSteamNetConnection_Invalid;
		break;
	}

	case k_ESteamNetworkingConnectionState_Connecting:
		break;

	case k_ESteamNetworkingConnectionState_Connected:
		GetLogManagerEx()->LogMsg("Connected to server OK");
		break;

	default:
		break;
	}
}

void Client::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	cl_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
}

void Client::PollConnectionStateChanges()
{
	cl_pCallbackInstance = this;
	m_pInterfaceCl->RunCallbacks();
}

Client* Client::cl_pCallbackInstance = nullptr;