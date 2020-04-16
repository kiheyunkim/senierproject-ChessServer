#ifndef _LOGINPACKET_H_
#define _LOGINPACKET_H_
#include "CriticalSection.h"
class LoginPacket : public Packet
{
private:
	LOGIN_PACKET_TYPE loginPacketType;

public:
	LoginPacket();
	LoginPacket(LOGIN_PACKET_TYPE packetType);
	virtual ~LoginPacket();

public:
	inline LOGIN_PACKET_TYPE GetLoginPacketType() { return loginPacketType; }

public:
	bool SetTypeToPacket();
	bool GetTypeFromPacket();

public:
	bool SetLoginData(const char* id, const char* pw, SOCKET client);
	bool SetJoinData(const char* id, const char* pw, const char* nick, SOCKET client);
	bool SetErrorType(LOGIN_ERROR_TYPE errorType, SOCKET client);

public:
	bool GetLoginData(char* id, char* pw, SOCKET& client);
	bool GetJoinData(char* id, char* pw, char* nick, SOCKET& client);
	bool GetErrorType(LOGIN_ERROR_TYPE& errorType, SOCKET& client);

public:
	bool SetCoreConnection(const char* passWord);
	bool GetCoreConnection(char* passWord);

public:
	virtual bool SetDataToStream(Stream& stream);
	virtual bool GetDataFromStream(Stream& stream, std::size_t length);
};

#endif // !_LOGINPACKET_H_