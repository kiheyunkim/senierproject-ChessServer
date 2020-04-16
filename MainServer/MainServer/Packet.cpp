#include "stdafx.h"
#include <cmath>
#include "IocpManager.h"
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
	: packetType(packetType), dataLength(0)
{
	memset(data, 0, BUFFER_SIZE);
}

Packet::~Packet() {}

PACKET_TYPE Packet::GetPacketType() const { return packetType; }

void Packet::SetPacketType(PACKET_TYPE packet)
{
	CriticalSectionSync sync(cs);

	packetType = packet;
}

bool Packet::SetDataToStream(Stream& stream)
{
	CriticalSectionSync sync(cs);

	char packetType[20]{ 0, };

	_itoa(static_cast<int>(GetPacketType()), packetType, 10);
	strncpy(stream.GetStream(), packetType, headerSize);
	memcpy(stream.GetStream() + headerSize, data, dataLength);

	stream.SetStreamLength(headerSize + dataLength);

	return true;
}

bool Packet::GetDataFromStream(Stream& stream, std::size_t length)
{
	CriticalSectionSync sync(cs);

	char packetType[20]{ 0, };

	strncpy(packetType, stream.GetStream(), headerSize);
	packetType[headerSize] = '\0';
	SetPacketType(static_cast<PACKET_TYPE>(atoi(packetType)));

	memcpy(data, stream.streamData + headerSize, length - headerSize);
	dataLength = length - headerSize;

	return true;
}
