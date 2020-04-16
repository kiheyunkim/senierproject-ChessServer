// MainServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "LogManager.h"
#include "NetworkSession.h"
#include "SessionManager.h"
#include "MySqlManager.h"
#include "IocpManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/// Global Managers
	sessionManager = new SessionManager;
	iocpManager = new IocpManager;
	mySqlManager = new MySqlManager;
	logManager = new LogManager(_T("log.txt"));

	std::wcout << _T("Server Start") << std::endl;

	if (false == logManager->LogThreadStart())
		return -1;

	if (false == iocpManager->InitializeIocp(9900))
		return -1;

	//if (false == mySqlManager->StartMySQL(3306)) ///< block here...
	//	return -1;

	//if (false == mySqlManager->StartQueryPool()) ///< block here...
	//	return -1;

	if (false == iocpManager->StartIoThreads())
		return -1;

	if (false == iocpManager->COnnectLoginServer("127.0.0.1", 8000))
		return -1;

	if (false == iocpManager->ConnectDBServer("127.0.0.1", 8090))
		return -1;

	if (false == iocpManager->StartAccept()) 
		return -1;

	iocpManager->CleanupIocp();

	delete iocpManager;
	delete sessionManager;
	delete mySqlManager;
	delete logManager;

	return 0;
}



