#include"stdafx.h"
#include "Packet.h"
#include <cmath>
#include "Stream.h"

std::size_t Stream::headerSize = static_cast<std::size_t>(log10(static_cast<int>(PACKET_TYPE::PACKET_TYPE_HEAD))) + 1;

void Stream::PullRemainedData(std::size_t pulledSize)
{
	if (GetStreamLength() == pulledSize)
		ResetStream();
	else
		memmove(streamData, streamData + pulledSize, streamLength);
}

Stream::Stream() :streamLength(0) { memset(streamData, 0, BUFFER_SIZE); }

Stream::Stream(const Packet& packet)
{
	char headTemp[20]{ 0, };

	_itoa(static_cast<int>(packet.GetPacketType()), headTemp, 10);
	strncpy(streamData, headTemp, headerSize);
	memcpy(streamData + headerSize, packet.GetData(), packet.GetPacketLength());

	streamLength = headerSize + packet.GetPacketLength();
}

Stream::~Stream() {}

char* Stream::GetStream() { return streamData; }

std::size_t Stream::GetStreamLength() { return streamLength; }

void Stream::SetStreamLength(std::size_t length) { streamLength = length; };

bool Stream::GetDataFromPacket(Packet* packet)
{
	if (packet == nullptr) return false;

	char headTemp[20]{ 0, };

	_itoa(static_cast<int>(packet->GetPacketType()), headTemp, 10);
	strncpy(streamData, headTemp, headerSize);
	memcpy(streamData + headerSize, packet->GetData(), packet->GetPacketLength());

	streamLength = headerSize + packet->GetPacketLength();

	return true;
}

bool Stream::SetDataToPacket(Packet* packet, std::size_t length) const
{
	char headTemp[20]{ 0, };

	strncpy(headTemp, streamData, headerSize);
	headTemp[headerSize] = '\0';
	packet->SetPacketType(static_cast<PACKET_TYPE>(atoi(headTemp)));

	if (!packet->SetData(streamData + headerSize, length - headerSize))
		return false;

	return true;
}

void Stream::RemoveSendedLength(std::size_t length)
{
	streamLength -= length;
	//PullRemainedData(length);
}

void Stream::ResetStream()
{
	memset(streamData, 0, 4096);
	streamLength = 0;
}

std::size_t Stream::GetFreeSize() { return 4096 - streamLength; }