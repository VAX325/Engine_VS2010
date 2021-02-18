#include <Base_include.h>
#include "NET.h"

#include "Main.h"
#include "Client/ClientPerems.h"
#include "Server/ServerPerems.h"
#include <iostream>
#include <chrono>
#include <thread>

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

//Utils funcs

SteamNetworkingMicroseconds g_logTimeZero = 0;

static void DebugOutput(ESteamNetworkingSocketsDebugOutputType eType, const char* pszMsg)
{
	SteamNetworkingMicroseconds time = SteamNetworkingUtils()->GetLocalTimestamp() - g_logTimeZero;
	GetLogObjCl()->LogMsg("%s\n", pszMsg);
	fflush(stdout);
	if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
	{
		fflush(stdout);
		fflush(stderr);
		ExitProcess(-1);
	}
}

static void InitSteamDatagramConnectionSockets()
{
	SteamDatagramErrMsg errMsg;
	if (!GameNetworkingSockets_Init(nullptr, errMsg))
		GetLogObjCl()->LogError("GameNetworkingSockets_Init failed.  %s", true, errMsg);

	g_logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();

	SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput);
}

static void ShutdownSteamDatagramConnectionSockets()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	GameNetworkingSockets_Kill();
}

void SteamGameSocketsInit()
{
	InitSteamDatagramConnectionSockets();
}

void SteamGameSocketsDeInit()
{
	ShutdownSteamDatagramConnectionSockets();
}
