#ifndef _MYSQLMANAGER_H_
#define _MYSQLMANAGER_H_
#include "MysqlQuery.h"
#include "CircularBuffer.h"
#include "CriticalSection.h"

#define DB_HOST					"localhost"
#define DB_USER					"dbadmin"
#define DB_PASSWD				"finalproject"
#define DB_NAME					"loginUsers"
#define DB_PORT					3306

class MysqlQuerySP;
class NetworkSession;
class MySqlManager
{
private:
	CriticalSection cs;

private:
	MYSQL* mySqlConnector;
	
private:
	CircularBuffer<std::pair<NetworkSession*, MysqlQuery*>> queryPool;
	HANDLE queryPoolHandle;

private:
	bool mySqlQuery(const char* query);
	static unsigned int WINAPI queryPoolThread(LPVOID lpParam);

public:
	MySqlManager();
	~MySqlManager();

public:
	bool StartMySQL(unsigned int port);
	bool StartQueryPool();

public:
	bool GetSqlResult(MYSQL_RES* result, MYSQL_ROW& row);
	bool GetIsThere();

public:
	bool RegisterQuery(NetworkSession* queryOwner, MysqlQuery* mysqlQuery);
	
};

extern MySqlManager* mySqlManager;

#endif // !_MYSQLMANAGER_H_
