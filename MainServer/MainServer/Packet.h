#ifndef _PACKET_H_
#define _PACKET_H_
#include "CriticalSection.h"
#include "Stream.h"
class PacketIO;

class Packet
{
private:
	static std::size_t headerSize;
	static std::size_t dataMaxLength;
	friend class PacketIO;

private:
	char data[BUFFER_SIZE];
	PACKET_TYPE packetType;
	std::size_t dataLength;

protected:
	CriticalSection cs;

public:
	Packet();
	virtual ~Packet();

protected:
	Packet(PACKET_TYPE packetType);

public:
	bool isFull(std::size_t size)	const;
	bool isEmpty()					const;

public:
	PACKET_TYPE GetPacketType()		const;
	void SetPacketType(PACKET_TYPE packetType);

public:
	virtual bool SetDataToStream(Stream& stream);
	virtual bool GetDataFromStream(Stream& stream, std::size_t length);
};

#endif // !_PACKET_H_
