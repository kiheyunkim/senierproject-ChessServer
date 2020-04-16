#include "stdafx.h"
#include "CriticalSection.h"
#include <mysql.h>
#include "MysqlQuery.h"
#include "Packet.h"
#include "Stream.h"
#include "Encryption.h"
#include "NetworkSession.h"
#include "MySqlManager.h"

MySqlManager* mySqlManager = nullptr;

MySqlManager::MySqlManager()
	:mySqlConnector(nullptr), queryPoolHandle(nullptr),queryPool(MAX_CONNECTION)
{}

MySqlManager::~MySqlManager() 
{
	queryPoolHandle = nullptr;
}

bool MySqlManager::mySqlQuery(const char* query)
{
	CriticalSectionSync sync(cs);

	if (query == nullptr) return false;

	if (mysql_query(mySqlConnector, query) == 0) return false;

	return true;
}

unsigned int WINAPI MySqlManager::queryPoolThread(LPVOID lpParam)
{
	assert(mySqlManager != nullptr);
	CriticalSection cs;

	//while (true)
	//{
	//	cs.Enter();
	//	Sleep(1);						///Control CPU Preemption
	//	
	//	if (mySqlManager->queryPool.isEmpty())
	//		continue;
	//	
	//	auto  ref = mySqlManager->queryPool.GetFront();
	//
	//	bool checkResultOK{ false };
	//	bool registerOK{ true };
	//
	//	switch (ref->second->GetType())
	//	{
	//	case QUERY_TYPE::LOGIN_REQUEST:
	//		ref->second->MakeQuery();
	//		mySqlManager->mySqlQuery(ref->second->GetStr());
	//		break;
	//
	//	case QUERY_TYPE::JOIN_REQUEST:
	//		ref->second->SetType(QUERY_TYPE::CHECK_ID);
	//		mySqlManager->mySqlQuery(ref->second->GetStr());
	//		if (checkResultOK = mySqlManager->GetIsThere())
	//			break;															//ID is Already Exist
	//
	//		ref->second->SetType(QUERY_TYPE::CHECK_NICK);
	//		mySqlManager->mySqlQuery(ref->second->GetStr());
	//		if (checkResultOK = mySqlManager->GetIsThere())
	//			break;															//Nick is Already Exist
	//		
	//		ref->second->SetType(QUERY_TYPE::JOIN_REQUEST);						//Register
	//		mySqlManager->mySqlQuery(ref->second->GetStr());
	//		
	//		ref->second->SetType(QUERY_TYPE::REGISTER_CHECK);
	//		mySqlManager->mySqlQuery(ref->second->GetStr());
	//		if ((registerOK = mySqlManager->GetIsThere()) == false)
	//			break;															//Register Fail
	//
	//		ref->second->SetType(QUERY_TYPE::JOIN_REQUEST);
	//		break;
	//	}
	//	
	//	bool result{ false };
	//	Packet packet(PACKET_TYPE::REQEUST_RESULT);
	//
	//	switch (ref->second->GetType())
	//	{
	//	case QUERY_TYPE::JOIN_REQUEST:
	//		result = (checkResultOK == false && registerOK == true) ? true : false;
	//		if (result)
	//			packet.SetRequestResult(ERROR_TYPE::JOIN_SUCCESS);
	//		break;
	//
	//	case QUERY_TYPE::CHECK_ID:
	//		assert(checkResultOK == true);
	//		packet.SetRequestResult(ERROR_TYPE::JOIN_ALREADY_ID_EXIST);
	//		break;
	//		
	//	case QUERY_TYPE::CHECK_NICK:
	//		assert(checkResultOK == true);
	//		packet.SetRequestResult(ERROR_TYPE::JOIN_ALREADY_NICK_EXIST);
	//		break;
	//
	//	case QUERY_TYPE::REGISTER_CHECK:
	//		assert(registerOK == false);
	//		packet.SetRequestResult(ERROR_TYPE::JOIN_FAILED);
	//		break;
	//	
	//	case QUERY_TYPE::LOGIN_REQUEST:
	//		result = mySqlManager->GetIsThere();
	//		packet.SetRequestResult(result ? ERROR_TYPE::LOGIN_SUCCESS : ERROR_TYPE::LOGIN_FAILED);
	//		ref->first->SetName(ref->second->GetId());
	//		break;
	//
	//	case QUERY_TYPE::RANK_REQUEST:
	//		break;
	//	default:
	//		break;
	//	}
	//
	//	ref->first->AddStream(packet);
	//	ref->first->PostSend();
	//
	//	delete ref->second;
	//	mySqlManager->queryPool.PopFront();
	//
	//	cs.Leave();
	//}

	return 0;
}

bool MySqlManager::StartMySQL(unsigned int port)
{
	CriticalSectionSync sync(cs);

	if ((mySqlConnector = mysql_init(nullptr)) == nullptr)
		return false;

	if ((mySqlConnector = mysql_real_connect(mySqlConnector, DB_HOST, DB_USER, DB_PASSWD, DB_NAME, port, nullptr, 0)) == nullptr)
		return false;

	logManager->WriteLog("[DEBUG] Sucess to Start Mysql");
	return true;
}

bool MySqlManager::StartQueryPool()
{
	CriticalSectionSync sync(cs);

	if ((queryPoolHandle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, queryPoolThread, nullptr, 0, nullptr))) == nullptr)
		return false;

	return true;
}

bool MySqlManager::GetSqlResult(MYSQL_RES* result, MYSQL_ROW& row)
{ 
	CriticalSectionSync sync(cs);

	if ((result = mysql_store_result(mySqlConnector)) == nullptr) return false;

	if ((row = mysql_fetch_row(result)) == nullptr) return false;

	return true; 
}

bool MySqlManager::GetIsThere()
{
	CriticalSectionSync sync(cs);

	MYSQL_RES* mysqlResult = nullptr;
	MYSQL_ROW mysqlRow = nullptr;

	GetSqlResult(mysqlResult, mysqlRow);

	if (mysqlRow == nullptr) 
		return false;
	
	if (atoi(mysqlRow[0]) == 1) 
		return true;

	return false;
}

bool MySqlManager::RegisterQuery(NetworkSession* queryOwner, MysqlQuery* mysqlQuery)
{
	CriticalSectionSync sync(cs);

	queryPool.PushBack(std::make_pair(queryOwner, mysqlQuery));

	return true;
}