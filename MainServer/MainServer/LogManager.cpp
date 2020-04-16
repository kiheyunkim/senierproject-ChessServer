#include "stdafx.h"
#include <tchar.h>
#include "CriticalSection.h"
#include "LogManager.h"

LogManager* logManager = nullptr;

LogManager::LogManager(TCHAR* fileName)
	:threadHandle(nullptr), logPool(MAX_CONNECTION)
{
	fileStream.open(fileName, std::ios::out | std::ios::app);
}

LogManager::~LogManager()
{
	fileStream.close();
}

bool LogManager::LogThreadStart()
{
	if (!fileStream.is_open()) return false;
	if (threadHandle != nullptr) return false;
	if (nullptr == (threadHandle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, logThread, nullptr, 0, nullptr)))) return false;

	logManager->WriteLog("[DEBUG] Sucess to Start Log Pool ");

	return true; 
}

bool LogManager::WriteLog(const char* logStr, ...)
{
	CriticalSectionSync sync(cs);

	SYSTEMTIME systemTime;
	va_list ap;

	char currentTime[32]{ 0, };
	char log[MAX_BUFFER_LENGTH]{ 0, };

	va_start(ap, logStr);
	vsprintf(log, logStr, ap);
	va_end(ap);

	GetLocalTime(&systemTime);
	sprintf(currentTime,"%02d-%02d-%02d %02d:%02d:%02d",
		systemTime.wYear,
		systemTime.wMonth,
		systemTime.wDay,
		systemTime.wHour,
		systemTime.wMinute,
		systemTime.wSecond);

	printf("[%s]\t%s\n", currentTime, log);

	std::string tempString;
	tempString += "[";
	tempString += currentTime;
	tempString += "]\t";
	tempString += log;
	tempString += "\n";

	logPool.PushBack(tempString);

	return true;
}

unsigned int WINAPI LogManager::logThread(LPVOID lpParam)
{
#ifdef _DEBUG
	assert(logManager != nullptr);
#else
	if (logManager == nullptr)
	{
		printf("Log Manager Error Occured Terminate Server");
		exit(-1);
	}
#endif
	CriticalSection cs;

	while (true)
	{
		Sleep(1);			///Control CPU Preemption
		cs.Enter();

		if (logManager->logPool.isEmpty())
		{
			cs.Leave();
			continue;
		}

		if (!logManager->fileStream.good())
		{
#ifdef _DEBUG
			assert(false);
#else
			printf("Log File has Error Terminate Server");
#endif
			cs.Leave();
		}

		logManager->fileStream << logManager->logPool.GetFront()->c_str();
		logManager->logPool.PopFront();

		cs.Leave();
	}

	return 0;
}