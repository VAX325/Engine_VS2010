#include "CLogManager.h"
#include <ctime>

CLogManager::CLogManager()
{
	int count = fs->GetCountOfFilesInFolder("../logs/", "log");

	string logName = "../logs/log";

	char buff[100];

	char* countc = _itoa(count, buff, 10);

	logName = logName + countc;

	logName = logName + ".log";

	fout.open( logName.c_str(), ios_base::out);

	fout.close();

	fout.open(logName.c_str(), ios_base::out);

	fout << "Logging strated!\n";
}

CLogManager::CLogManager(const CLogManager &)
{
	int count = fs->GetCountOfFilesInFolder("../logs/", "log");

	string logName = "../logs/log";

	char buff[100];

	char* countc = _itoa(count, buff, 10);

	logName = logName + countc;

	logName = logName + ".log";

	fout.open(logName.c_str(), ios_base::out);

	fout.close();

	fout.open(logName.c_str(), ios_base::out);

	fout << "Logging strated!\n";
};

CLogManager::~CLogManager()
{
	fout.close();
}

void CLogManager::LogMsg(char* Msg)
{
	time_t now = time(0);

	tm* local = localtime(&now);

	fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " Msg: " << Msg << "\n";
}

void CLogManager::LogError(char* Msg, bool needToShutdown)
{
	time_t now = time(0);

	tm* local = localtime(&now);

	if (needToShutdown)
	{
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR: " << Msg << "\n";
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " SHUTING DOWN! " << "\n";

		fout.close();
		exit(-1);
	}
	else
	{
		fout << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << " ERROR: " << Msg << "\n";
	}
}