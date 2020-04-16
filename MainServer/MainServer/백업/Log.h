#ifndef _LOG_H_
#define _LOG_H_
#include <fstream>
#include "stdafx.h"

class Log
{
public:
	static bool WriteLog(CONST TCHAR* str, ...)
	{
		SYSTEMTIME systemTime;
		std::wofstream wofStream;

		va_list ap;
		TCHAR currentTime[32] = { 0, };
		TCHAR log[MAX_BUFFER_LENGTH] = { 0, };

		va_start(ap, str);
		_vstprintf(log, str, ap);
		va_end(ap);

		GetLocalTime(&systemTime);
		_sntprintf(currentTime, 32, _T("%02d-%02d-%02d %02d:%02d:%02d"),
			systemTime.wYear,
			systemTime.wMonth,
			systemTime.wDay,
			systemTime.wHour,
			systemTime.wMinute,
			systemTime.wSecond);

		_tprintf(_T("[%s]\t%s\n"), currentTime, log);
		//std::wcout << _T("[") << currentTime << _T("]") << _T("\t") << log << _T("\n");

		wofStream.open("log.txt", std::ios::out | std::ios::app);
		if (!wofStream.good()) return false;

		wofStream << _T("[") << currentTime << _T("]\t") << log << _T("\n");

		wofStream.close();
		return true;
	}
};

#endif // !_LOG_H_
