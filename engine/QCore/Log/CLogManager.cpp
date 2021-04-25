#include <Base_include.h>
#include "CLogManager.h"

#define DEBUG_INVOKE __asm int 3

#include <ctime>

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

	string* superString = new string(logName.c_str());

	CurentLog = superString;

	fout = new ofstream(logName.c_str());

	//fout->open(logName.c_str(), ios_base::out);

	(*fout) << "Logging strated!\n";

	fout->flush();
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

	(*fout) << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " Msg: " << MsgBuff << "\n";

	fout->flush();

#ifdef _DEBUG
	printf("%s\n" ,MsgBuff);
#endif // _DEBUG

	free(MsgBuff);
}

void CLogManager::LogError(const char* Msg, bool needToShutdown, ...)
{
	if(needToShutdown)
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
		(*fout) << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR: " << MsgBuff << "\n";
		(*fout) << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " SHUTING DOWN! " << "\n";

#ifdef _DEBUG
		printf("%s\n", MsgBuff);
#endif // _DEBUG

		fout->close();
		exit(0xAcAcAcA);
	}
	else
	{
		(*fout) << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR: " << MsgBuff << "\n";

		fout->flush();
#ifdef _DEBUG
		printf("%s\n", MsgBuff);
#endif // _DEBUG
	}

	free(MsgBuff);
}

CLogManager* LogManager;

void InitLogManager(bool IsServer)
{
	LogManager = new CLogManager();
	LogManager->Init(IsServer);
}

inline CLogManager* GetLogManager()
{
	return LogManager;
}

CLogManager* GetLogManagerEx()
{
	return GetLogManager();
}