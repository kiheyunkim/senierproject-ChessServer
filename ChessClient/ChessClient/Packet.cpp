#include "stdafx.h"
#include <cmath>
#include "Stream.h"
#include "PacketIO.h"
#include "Packet.h"

std::size_t Packet::headerSize = static_cast<std::size_t>(log10(static_cast<int>(PACKET_TYPE::PACKET_TYPE_HEAD))) + 1;

//headerSize + ' ' + '\0' -> dataMaxLength = BUFFER_SIZE - headerSize - ' '(1) - '\0'(1);
std::size_t Packet::dataMaxLength = BUFFER_SIZE - headerSize - 2;

bool Packet::isFull(std::size_t size) const { return dataLength + size > dataMaxLength; }

bool Packet::isEmpty() const { return dataLength == 0; }

Packet::Packet() :dataLength(0)
{
	memset(data, 0, BUFFER_SIZE);
}

Packet::Packet(PACKET_TYPE packetType)
	: packetType(packetType),
	dataLength(0)
{
	memset(data, 0, BUFFER_SIZE);
}

Packet::~Packet() {}

PACKET_TYPE Packet::GetPacketType() const { return packetType; }

const char* Packet::GetData() const { return data; }

std::size_t Packet::GetPacketLength() const { return dataLength; }

bool Packet::SetData(const char* data, std::size_t length)
{
	if (data == nullptr || length == 0) return false;

	memcpy(this->data, data, length);
	dataLength = length;

	return true;
}

void Packet::SetPacketType(PACKET_TYPE packet) { packetType = packet; }

bool Packet::SetLoginData(const char* id, const char* pw)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::LOGIN_REQUEST) return false;
	if (id == nullptr || pw == nullptr) return false;

	PacketIO packetIO;

	int idSize = static_cast<int>(strlen(id));
	if (!packetIO.WriteString(this, id, idSize)) return false;
	if (!packetIO.WriteInt(this, idSize)) return false;

	int pwSize = static_cast<int>(strlen(pw));
	if (!packetIO.WriteString(this, pw, pwSize)) return false;
	if (!packetIO.WriteInt(this, pwSize)) return false;

	return true;
}

bool Packet::SetJoinData(const char* id, const char* pw, const char* nick)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::JOIN_REQUEST) return false;
	if (id == nullptr || pw == nullptr || nick == nullptr) return false;

	PacketIO packetIO;

	int idSize = static_cast<int>(strlen(id));
	if (!packetIO.WriteString(this, id, idSize)) return false;
	if (!packetIO.WriteInt(this, idSize)) return false;

	int pwSize = static_cast<int>(strlen(pw));
	if (!packetIO.WriteString(this, pw, pwSize)) return false;
	if (!packetIO.WriteInt(this, pwSize)) return false;

	int nickSize = static_cast<int>(strlen(nick));
	if (!packetIO.WriteString(this, nick, nickSize)) return false;
	if (!packetIO.WriteInt(this, nickSize)) return false;

	return true;
}

bool Packet::SetMessageData(std::size_t len, const char* message)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::MESSAGE_BROADCAST) return false;
	if (len == 0 || message == nullptr) return false;

	PacketIO packetIO;

	int messageSize = static_cast<int>(strlen(message));
	if (!packetIO.WriteString(this, message, messageSize)) return false;
	if (!packetIO.WriteInt(this, messageSize)) return false;

	return true;
}

bool Packet::SetRequestResult(ERROR_TYPE errorType)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::REQEUST_RESULT) return false;

	PacketIO packetIO;
	if (!packetIO.WriteInt(this, static_cast<int>(errorType))) return false;

	return true;
}


bool Packet::GetLoginData(char* id, char* pw)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::LOGIN_REQUEST) return false;
	PacketIO packetIO;

	int readingSize{};
	if (!packetIO.ReadInt(this, readingSize)) return false;
	if (!packetIO.ReadString(this, pw, readingSize)) return false;

	if (!packetIO.ReadInt(this, readingSize)) return false;
	if (!packetIO.ReadString(this, id, readingSize)) return false;

	return true;
}

bool Packet::GetJoinData(char* id, char* pw, char* nick)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::JOIN_REQUEST) return false;
	PacketIO packetIO;

	int readingSize{};
	if (!packetIO.ReadInt(this, readingSize)) return false;
	if (!packetIO.ReadString(this, nick, readingSize)) return false;

	if (!packetIO.ReadInt(this, readingSize)) return false;
	if (!packetIO.ReadString(this, pw, readingSize)) return false;

	if (!packetIO.ReadInt(this, readingSize)) return false;
	if (!packetIO.ReadString(this, id, readingSize)) return false;

	return true;
}

bool Packet::GetMessageData(char* message)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::MESSAGE_BROADCAST) return false;
	PacketIO packetIO;

	int readingSize{};
	if (!packetIO.ReadInt(this, readingSize)) return false;
	if (!packetIO.ReadString(this, message, readingSize)) return false;

	return true;
}

bool Packet::GetRequestResult(ERROR_TYPE& errorType)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::REQEUST_RESULT) return false;

	PacketIO packetIO;

	int errorTypeValue{};
	if (!packetIO.ReadInt(this, errorTypeValue)) return false;
	errorType = static_cast<ERROR_TYPE>(errorTypeValue);

	return true;
}


bool Packet::SetMovingPeice(TEAM_TYPE teamType, PIECE_TYPE pieceType, int movePos)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::PIECE_MOVING) return false;
	PacketIO packetIO;

	if (!packetIO.WriteInt(this, static_cast<int>(teamType))) return false;
	if (!packetIO.WriteInt(this, static_cast<int>(pieceType))) return false;
	if (!packetIO.WriteInt(this, movePos)) return false;

	return true;
}

bool Packet::SetMovingPieceRequest(PIECE_TYPE pieceType, int currentPos, int movePos)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::PIECE_MOVING_REQUEST) return false;
	PacketIO packetIO;

	if (!packetIO.WriteInt(this, static_cast<int>(pieceType))) return false;
	if (!packetIO.WriteInt(this, currentPos)) return false;
	if (!packetIO.WriteInt(this, movePos)) return false;

	return true;
}

bool Packet::GetMovingPeice(TEAM_TYPE& teamType, PIECE_TYPE& pieceType, int& movePos)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::PIECE_MOVING) return false;
	PacketIO packetIO;

	int piece{}, team{};
	if (!packetIO.ReadInt(this, movePos)) return false;
	if (!packetIO.ReadInt(this, piece)) return false;
	if (!packetIO.ReadInt(this, team)) return false;

	pieceType = static_cast<PIECE_TYPE>(piece);
	teamType = static_cast<TEAM_TYPE>(team);

	return true;
}

bool Packet::GetMovingPieceRequest(PIECE_TYPE& pieceType, int& currentPos, int& movePos)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::PIECE_MOVING_REQUEST) return false;
	PacketIO packetIO;

	int piece{};
	if (!packetIO.ReadInt(this, movePos)) return false;
	if (!packetIO.ReadInt(this, currentPos)) return false;
	if (!packetIO.ReadInt(this, piece)) return false;

	pieceType = static_cast<PIECE_TYPE>(piece);

	return true;
}

bool Packet::SetRequestedRoomList()
{
	CriticalSectionSync sync(cs);
	///Not for Client
	//if (packetType != PACKET_TYPE::ROOM_LIST_REQUEST) return false;
	//if (iocpManager == nullptr) return false;

	//PacketIO packetIO;
	//int roomSize = iocpManager->GetGameRoomMgr()->GetCurrentRoomSize() > 5 ? 5 : iocpManager->GetGameRoomMgr()->GetCurrentRoomSize();	//최대 5개로 제한
	//for (int i = 0; i < roomSize; i++)
	//	if (!packetIO.WriteInt(this, iocpManager->GetGameRoomMgr()->GetGameRoomWithNumber(static_cast<std::size_t>(i))->GetRoomCount())) return false;

	//if (!packetIO.WriteInt(this, roomSize)) return false;

	return true;
}

bool Packet::SetRoomJoinRequest(int roomNumber)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::ROOM_JOIN_REQUEST) return false;
	PacketIO packetIO;

	if (packetIO.WriteInt(this, roomNumber)) return false;

	return true;
}

bool Packet::GetRequestedRoomList(std::stack<int>& roomList)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::ROOM_LIST_REQUEST) return false;
	PacketIO packetIO;

	int size{};
	if (!packetIO.ReadInt(this, size)) return false;

	for (int i = 0; i < size; i++)
	{
		int value{};
		if (!packetIO.ReadInt(this, value)) return false;
		roomList.push(value);
	}

	return true;
}

bool Packet::GetRoomJoinRequest(int& roomNumber)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::ROOM_JOIN_REQUEST) return false;
	PacketIO packetIO;

	if (packetIO.ReadInt(this, roomNumber)) return false;

	return true;
}

bool Packet::SetReadyState(bool currentReadyState)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::READY_REQUEST) return false;
	PacketIO packetIO;

	if (!packetIO.WriteBool(this, currentReadyState)) return false;

	return true;
}

bool Packet::GetReadyState(bool& currentReadyState)
{
	CriticalSectionSync sync(cs);

	if (packetType != PACKET_TYPE::READY_REQUEST) return false;
	PacketIO packetIO;

	if (!packetIO.ReadBool(this, currentReadyState)) return false;

	return true;
}