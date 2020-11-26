#include "CLogManager.h"
#include <Base_include.h>
#include <ctime>

void CLogManager::Init()
{
	int count = fs->GetCountOfFilesInFolder((char*)"../logs/", (char*)"log");

	string logName = "../logs/log";

	char buff[100];

	char* countc = _itoa(count, buff, 10);

	logName = logName + countc;

	logName = logName + ".log";

	if (CurentLog == logName || CurentLog.empty())
	{
		CurentLog = logName;

		fout.open(logName.c_str(), ios_base::out);

		fout << "Logging strated!\n";
	}
}

void CLogManager::LogMsg(char* Msg)
{
	time_t now = time(0);

	tm* local = localtime(&now);

	fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " Msg: " << Msg << "\n";

#ifdef _DEBUG
	printf("%s\n" ,Msg);
#endif // _DEBUG

}

void CLogManager::LogError(char* Msg, bool needToShutdown)
{
	time_t now = time(0);

	tm* local = localtime(&now);

	if (needToShutdown)
	{
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR: " << Msg << "\n";
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " SHUTING DOWN! " << "\n";

#ifdef _DEBUG
		printf("%s\n", Msg);
#endif // _DEBUG

		fout.close();
		exit(0xAcAcAcA);
	}
	else
	{
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR: " << Msg << "\n";

#ifdef _DEBUG
		printf("%s\n", Msg);
#endif // _DEBUG
	}
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

#ifdef _DEBUG
			printf("%s\n", Msg);
#endif // _DEBUG
		}
	}
}

void CLogManager::LogErrorLua(lua_State* L)
{
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

#ifdef _DEBUG
		printf("%s\n", Msg);
#endif // _DEBUG
	}
}