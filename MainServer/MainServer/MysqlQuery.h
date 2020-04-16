#ifndef _MYSQLQUERY_H_
#define _MYSQLQUERY_H_
#include "CriticalSection.h"

class MysqlQuery
{
private:
	char* query;
	const std::size_t maxSize;

private:
	CriticalSection cs;

private:
	QUERY_TYPE type;
	std::string identify;
	std::string passwd;
	std::string nickName;

public:
	MysqlQuery();
	MysqlQuery(QUERY_TYPE type);
	~MysqlQuery();


public:
	const char* GetId() const;
	const char* GetStr() const;
	QUERY_TYPE GetType() const;
	void SetType(QUERY_TYPE type);
	bool SetLoginQuery(const char* id, const char* pw);
	bool SetJoinQuery(const char* id, const char* pw, const char* nick);
	bool MakeQuery();
};

#endif // !_MYSQLQUERY_H_