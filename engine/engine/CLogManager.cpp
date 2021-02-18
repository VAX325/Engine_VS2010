#include "CLogManager.h"
#include <Base_include.h>

#define DEBUG_INVOKE __asm int 3

#include <ctime>

#include "Main.h"

void CLogManager::Init(bool IsServer)
{
	int count = fs->GetCountOfFilesInFolder((char*)"../logs/", (char*)"log");

	string logName;

	if (IsServer)
		logName = "../logs_sv/log";
	else
		logName = "../logs/log";

	char buff[100];

	char* countc = _itoa(count, buff, 10);

	logName = logName + countc;

	logName = logName + ".log";

	if (CurentLog == logName || CurentLog.empty())
	{
		CurentLog = logName;

		fout.open(logName.c_str(), ios_base::out);

		fout << "Logging strated!\n";

		fout.flush();
	}
}

void CLogManager::LogMsg(const char* Msg, ...)
{
	time_t now = time(0);

	tm* local = localtime(&now);

	char* MsgBuff = (char*)malloc(128 * sizeof(*MsgBuff));
	MsgBuff[0] = '\0';

	va_list arg;
	va_start(arg, Msg);
	vsprintf((char*)MsgBuff, Msg, arg);
	va_end(arg);

	fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " Msg: " << MsgBuff << "\n";

	fout.flush();

#ifdef _DEBUG
	printf("%s\n" ,MsgBuff);
#endif // _DEBUG

	free(MsgBuff);
}

void CLogManager::LogError(const char* Msg, bool needToShutdown, ...)
{
	DEBUG_INVOKE

	time_t now = time(0);

	tm* local = localtime(&now);

	char* MsgBuff = (char*)malloc(128 * sizeof(*MsgBuff));
	MsgBuff[0] = '\0';

	va_list arg;
	va_start(arg, Msg);
	vsprintf((char*)MsgBuff, Msg, arg);
	va_end(arg);

	if (needToShutdown)
	{
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR: " << MsgBuff << "\n";
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " SHUTING DOWN! " << "\n";

#ifdef _DEBUG
		printf("%s\n", MsgBuff);
#endif // _DEBUG

		fout.close();
		exit(0xAcAcAcA);
	}
	else
	{
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR: " << MsgBuff << "\n";

		fout.flush();
#ifdef _DEBUG
		printf("%s\n", MsgBuff);
#endif // _DEBUG
	}

	free(MsgBuff);
}

void CLogManager::LogMsgLua(lua_State* L)
{
	int count = lua_gettop(L); // получаем количество переданных параметров
	if (count < 1)
	{ 
		LogError((char*)"Internal LUA error", true);
	}

	for (int j = 0; j < count; j++)
	{
		if (lua_isstring(L, j + 1))
		{
			char* Msg = (char*)lua_tostring(L, j + 1);

			time_t now = time(0);

			tm* local = localtime(&now);

			fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " Msg LUA: " << Msg << "\n";

			fout.flush();
#ifdef _DEBUG
			printf("%s\n", Msg);
#endif // _DEBUG
		}
	}
}

void CLogManager::LogErrorLua(lua_State* L)
{
	DEBUG_INVOKE

	int count = lua_gettop(L); // получаем количество переданных параметров
	if (count < 1)
	{
		LogError((char*)"Internal LUA error", true);
	}
		
	char* Msg = (char*)"";
	bool needToShutdown = false;

	for (int j = 0; j < count; j++)
	{
		if (lua_isstring(L, j + 1))
		{
			Msg = (char*)lua_tostring(L, j + 1);
		}
		if (lua_isboolean(L, j + 2)) 
		{
			needToShutdown = lua_toboolean(L, j + 2);
		}
	}

	time_t now = time(0);

	tm* local = localtime(&now);

	if (needToShutdown)
	{
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR LUA: " << Msg << "\n";
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " SHUTING DOWN! " << "\n";

#ifdef _DEBUG
		printf("%s\n", Msg);
#endif // _DEBUG

		fout.close();
		exit(0xAcAcAcA);
	}
	else
	{
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR LUA: " << Msg << "\n";
		
		fout.flush();

#ifdef _DEBUG
		printf("%s\n", Msg);
#endif // _DEBUG
	}
}

#include "Client/Client.h"
#include "Server/Server.h"

CLogManager* GetLogManager()
{
	if(IsClient())
	{
		return GetLogObjCl();
	}
	else
	{
		return GetLogObj();
	}
}