#ifndef _PACKET_H_
#define _PACKET_H_
#include "CriticalSection.h"
class PacketIO;

class Packet
{
private:
	CriticalSection cs;

private:
	static std::size_t headerSize;
	static std::size_t dataMaxLength;
	friend class PacketIO;

private:
	char data[BUFFER_SIZE];
	PACKET_TYPE packetType;
	std::size_t dataLength;

private:
	bool isFull(std::size_t size)	const;
	bool isEmpty()					const;

public:
	Packet();
	Packet(PACKET_TYPE packetType);
	~Packet();

public:
	PACKET_TYPE GetPacketType()		const;
	const char* GetData()			const;
	std::size_t GetPacketLength()	const;
	bool SetData(const char* data, std::size_t length);

public:
	void SetPacketType(PACKET_TYPE packetType);

	bool SetLoginData(const char* id, const char* pw);
	bool SetJoinData(const char* id, const char* pw, const char* nick);
	bool SetMessageData(std::size_t len, const char* message);
	bool SetRequestResult(ERROR_TYPE errorType);

	bool GetLoginData(char* id, char* pw);
	bool GetJoinData(char* id, char* pw, char* nick);
	bool GetMessageData(char* message);
	bool GetRequestResult(ERROR_TYPE& errorType);

public:
	bool SetMovingPeice(TEAM_TYPE teamType, PIECE_TYPE pieceType, int movePos);
	bool SetMovingPieceRequest(PIECE_TYPE pieceType, int currentPos, int movePos);

	bool GetMovingPeice(TEAM_TYPE& teamType, PIECE_TYPE& pieceType, int& movePos);
	bool GetMovingPieceRequest(PIECE_TYPE& pieceType, int& currentPos, int& movePos);

	bool SetRequestedRoomList();
	bool SetRoomJoinRequest(int roomNumber);

	bool GetRequestedRoomList(std::stack<int>& roomList);
	bool GetRoomJoinRequest(int& roomNumber);

	bool SetReadyState(bool currentReadyState);
	bool GetReadyState(bool& currentReadSate);
};

#endif // !_PACKET_H_
