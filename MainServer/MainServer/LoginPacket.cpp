#include "stdafx.h"
#include "LoginPacket.h"
#include "Packet.h"
#include "PacketIO.h"

LoginPacket::LoginPacket()
	:Packet(PACKET_TYPE::LOGIN_SERVER), loginPacketType(LOGIN_PACKET_TYPE::LOGIN_PACKET_TYPE_HEADER) {}

LoginPacket::LoginPacket(LOGIN_PACKET_TYPE packetType)
	: Packet(PACKET_TYPE::LOGIN_SERVER), loginPacketType(packetType) {}

LoginPacket::~LoginPacket() {}

bool LoginPacket::SetTypeToPacket()
{
	CriticalSectionSync sync(cs);

	if (loginPacketType == LOGIN_PACKET_TYPE::LOGIN_PACKET_TYPE_HEADER) return false;

	PacketIO packetIO;

	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(loginPacketType))) return false;

	return true;
}

bool LoginPacket::GetTypeFromPacket()
{
	CriticalSectionSync sync(cs);

	PacketIO packetIO;

	int loginHeader{ 0 };
	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), loginHeader))return false;

	loginPacketType = static_cast<LOGIN_PACKET_TYPE>(loginHeader);

	return true;
}

bool LoginPacket::SetLoginData(const char* id, const char* pw, SOCKET client)
{
	CriticalSectionSync sync(cs);

	if (GetPacketType() != PACKET_TYPE::LOGIN_SERVER) return false;
	if (id == nullptr || pw == nullptr) return false;

	PacketIO packetIO;

	int idLength{ static_cast<int>(strlen(id)) };
	if (!packetIO.WriteString(dynamic_cast<Packet*>(this), id, idLength)) return false;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), idLength)) return false;

	int pwLength{ static_cast<int>(strlen(pw)) };
	if (!packetIO.WriteString(dynamic_cast<Packet*>(this), pw, pwLength)) return false;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), pwLength)) return false;

	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(client))) return false;

	loginPacketType = LOGIN_PACKET_TYPE::LOGIN_REQUEST;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(loginPacketType))) return false;

	return true;
}

bool LoginPacket::SetJoinData(const char* id, const char* pw, const char* nick, SOCKET client)
{
	CriticalSectionSync sync(cs);

	if (GetPacketType() != PACKET_TYPE::LOGIN_SERVER) return false;
	if (id == nullptr || pw == nullptr || nick == nullptr) return false;

	PacketIO packetIO;

	int idLength{ static_cast<int>(strlen(id)) };
	if (!packetIO.WriteString(dynamic_cast<Packet*>(this), id, idLength)) return false;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), idLength)) return false;

	int pwLength{ static_cast<int>(strlen(pw)) };
	if (!packetIO.WriteString(dynamic_cast<Packet*>(this), pw, pwLength)) return false;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), pwLength)) return false;

	int nickLength{ static_cast<int>(strlen(nick)) };
	if (!packetIO.WriteString(dynamic_cast<Packet*>(this), nick, nickLength)) return false;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), nickLength)) return false;

	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(client))) return false;

	loginPacketType = LOGIN_PACKET_TYPE::JOIN_REQUEST;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(loginPacketType))) return false;

	return true;
}

bool LoginPacket::SetErrorType(LOGIN_ERROR_TYPE errorType, SOCKET client)
{
	CriticalSectionSync sync(cs);
	if (GetPacketType() != PACKET_TYPE::LOGIN_SERVER) return false;

	PacketIO packetIO;

	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(errorType))) return false;

	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(client))) return false;

	loginPacketType = LOGIN_PACKET_TYPE::REQUEST_RESULT;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(loginPacketType))) return false;

	return true;
}

bool LoginPacket::GetLoginData(char* id, char* pw, SOCKET& client)
{
	CriticalSectionSync sync(cs);
	PacketIO packetIO;

	if (GetPacketType() != PACKET_TYPE::LOGIN_SERVER) return false;
	if (loginPacketType != LOGIN_PACKET_TYPE::LOGIN_REQUEST) return false;

	int clientSocket{ 0 };
	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), clientSocket)) return false;
	client = static_cast<SOCKET>(clientSocket);

	int readingSize{ 0 };
	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), readingSize)) return false;
	if (!packetIO.ReadString(dynamic_cast<Packet*>(this), pw, readingSize)) return false;

	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), readingSize)) return false;
	if (!packetIO.ReadString(dynamic_cast<Packet*>(this), id, readingSize)) return false;

	return true;
}

bool LoginPacket::GetJoinData(char* id, char* pw, char* nick, SOCKET& client)
{
	CriticalSectionSync sync(cs);
	PacketIO packetIO;

	if (GetPacketType() != PACKET_TYPE::LOGIN_SERVER) return false;
	if (loginPacketType != LOGIN_PACKET_TYPE::JOIN_REQUEST) return false;

	int clientSocket{ 0 };
	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), clientSocket)) return false;
	client = static_cast<SOCKET>(clientSocket);

	int readingSize{ 0 };
	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), readingSize)) return false;
	if (!packetIO.ReadString(dynamic_cast<Packet*>(this), nick, readingSize)) return false;

	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), readingSize)) return false;
	if (!packetIO.ReadString(dynamic_cast<Packet*>(this), pw, readingSize)) return false;

	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), readingSize)) return false;
	if (!packetIO.ReadString(dynamic_cast<Packet*>(this), id, readingSize)) return false;

	return true;
}

bool LoginPacket::GetErrorType(LOGIN_ERROR_TYPE& errorType, SOCKET& client)
{
	CriticalSectionSync sync(cs);
	PacketIO packetIO;

	if (GetPacketType() != PACKET_TYPE::LOGIN_SERVER) return false;
	if (loginPacketType != LOGIN_PACKET_TYPE::REQUEST_RESULT) return false;

	int clientSocket{ 0 };
	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), clientSocket)) return false;
	client = static_cast<SOCKET>(clientSocket);

	int readInt{ 0 };
	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), readInt)) return false;

	errorType = static_cast<LOGIN_ERROR_TYPE>(readInt);

	return true;
}

bool LoginPacket::SetCoreConnection(const char* passWord)
{
	CriticalSectionSync sync(cs);

	if (GetPacketType() != PACKET_TYPE::LOGIN_SERVER) return false;
	if (passWord == nullptr) return false;

	PacketIO packetIO;

	int passLength{ static_cast<int>(strlen(passWord)) };
	if (!packetIO.WriteString(dynamic_cast<Packet*>(this), passWord, passLength)) return false;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), passLength)) return false;

	loginPacketType = LOGIN_PACKET_TYPE::CORE_CONNECTION;
	if (!packetIO.WriteInt(dynamic_cast<Packet*>(this), static_cast<int>(loginPacketType))) return false;

	return true;
}

bool LoginPacket::GetCoreConnection(char* passWord)
{
	CriticalSectionSync sync(cs);

	if (GetPacketType() != PACKET_TYPE::LOGIN_SERVER) return false;
	if (loginPacketType != LOGIN_PACKET_TYPE::CORE_CONNECTION) return false;

	PacketIO packetIO;

	int readingSize{ 0 };
	if (!packetIO.ReadInt(dynamic_cast<Packet*>(this), readingSize)) return false;
	if (!packetIO.ReadString(dynamic_cast<Packet*>(this), passWord, readingSize)) return false;

	return true;
}

bool LoginPacket::SetDataToStream(Stream& stream)
{
	if (!SetTypeToPacket()) return false;

	if (!Packet::SetDataToStream(stream)) return false;

	return true;
}

bool LoginPacket::GetDataFromStream(Stream& stream, std::size_t length)
{
	if (!Packet::GetDataFromStream(stream, length)) return false;

	if (!GetTypeFromPacket()) return false;

	return true;
}