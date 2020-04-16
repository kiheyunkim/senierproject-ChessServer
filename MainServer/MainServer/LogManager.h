#ifndef _LOGMANAGER_H_
#define _LOGMANAGER_H_

#include <fstream>
#include "CircularBuffer.h"
#include "CriticalSection.h"

class LogManager
{
private:
	CriticalSection cs;

private:
	std::ofstream fileStream;

private:
	CircularBuffer<std::string> logPool;
	HANDLE threadHandle;

public:
	LogManager(TCHAR* fileName);
	~LogManager();
	bool LogThreadStart();
	bool WriteLog(const char* logStr, ...);

public:
	static unsigned int WINAPI logThread(LPVOID lpParam);
};

extern LogManager* logManager;

#endif // !_LOGMANAGER_H_
