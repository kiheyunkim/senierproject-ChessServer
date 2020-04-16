// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#ifndef _STDAFX_H_
#define _STDAFX_H_
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"libmysql")
#pragma comment(lib,"mswsock.lib")

#define MAX_BUFFER_LENGTH 4096
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "targetver.h"

#include <cstdio>
#include <tchar.h>
#include <map>

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>

#include <mysql.h>
#include <process.h>
#include <cassert>
//#include <vector>
#include <memory>
#include <list>
#include <stack>
#include <queue>

#include "LogManager.h"
#include "MySql.h"
#include "Protocol.h"
#include "LoginServerProtocol.h"
#include "Packet.h"
#include "Stream.h"



#endif // !_STDAFX_H_ // TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.