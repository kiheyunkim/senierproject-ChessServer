#include "stdafx.h"
#include "Encryption.h"
#include "MySqlManager.h"
#include "MysqlQuery.h"

MysqlQuery::MysqlQuery()
	:query(nullptr), maxSize(1024) 
{
	query = new char[maxSize];
	memset(query, 0, maxSize);
}

MysqlQuery::MysqlQuery(QUERY_TYPE type)
:type(type), query(nullptr), maxSize(1024)
{
	query = new char[maxSize];
	memset(query, 0, maxSize);
}

MysqlQuery::~MysqlQuery()
{
	delete[] query;
}

const char* MysqlQuery::GetId() const
{
	return identify.c_str();
}

const char* MysqlQuery::GetStr() const
{
	return query;
}

void MysqlQuery::SetType(QUERY_TYPE queryType)
{
	CriticalSectionSync Sync(cs);

	type = queryType;
	MakeQuery();
}

QUERY_TYPE MysqlQuery::GetType() const
{

	return type;
}

bool MysqlQuery::SetLoginQuery(const char* id, const char* pw)
{
	CriticalSectionSync Sync(cs);

	if (id == nullptr || pw == nullptr) return false;

	type = QUERY_TYPE::LOGIN_REQUEST;
	identify = id;
	passwd = pw;

	return true;
}

bool MysqlQuery::SetJoinQuery(const char* id, const char* pw, const char* nick)
{
	CriticalSectionSync Sync(cs);

	if (id == nullptr || pw == nullptr || nick == nullptr) return false;

	type = QUERY_TYPE::JOIN_REQUEST;
	identify = id;
	passwd = pw;
	nickName = nick;

	return true;
}

bool MysqlQuery::MakeQuery()
{
	CriticalSectionSync Sync(cs);
	Encryption encrypt;

	std::string tempQuery;

	switch (type)
	{
	case QUERY_TYPE::CHECK_ID:
		tempQuery = "select count(*) from user where id ='";
		tempQuery += identify;
		tempQuery += "';";
		memset(query, 0, maxSize);
		strcpy(query, tempQuery.c_str());
		break;
	case QUERY_TYPE::CHECK_NICK:
		tempQuery = "select count(*) from user where nickname ='";
		tempQuery += nickName;
		tempQuery += "';";
		memset(query, 0, maxSize);
		strcpy(query, tempQuery.c_str());
		break;
	case QUERY_TYPE::LOGIN_REQUEST:
	case QUERY_TYPE::REGISTER_CHECK:
		tempQuery = "select count(*) from user where id ='";
		tempQuery += identify;
		tempQuery += "' AND passwd = '";
		tempQuery += encrypt.EncryptWithSHA256(passwd);
		tempQuery += "';";
		memset(query, 0, maxSize);
		strcpy(query, tempQuery.c_str());
		break;
	case QUERY_TYPE::JOIN_REQUEST:
		tempQuery = "insert into user (id,passwd,nickname) values('";
		tempQuery += identify;
		tempQuery += "','";
		tempQuery += encrypt.EncryptWithSHA256(passwd);
		tempQuery += "','";
		tempQuery += nickName;
		tempQuery += "');";
		memset(query, 0, maxSize);
		strcpy(query, tempQuery.c_str());
		break;
	case QUERY_TYPE::RANK_REQUEST:
		break;
	default:
		break;
	}

	return true;
}



