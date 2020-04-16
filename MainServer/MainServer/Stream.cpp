#include "stdafx.h"
#include "Packet.h"
#include <cmath>
#include "Protocol.h"
#include "Stream.h"

std::size_t Stream::headerSize = static_cast<std::size_t>(log10(static_cast<int>(PACKET_TYPE::PACKET_TYPE_HEAD))) + 1;

void Stream::PullRemainedData(std::size_t pulledSize)
{
	CriticalSectionSync sync(cs);

	if (GetStreamLength() == pulledSize)
		ResetStream();
	else
		memmove(streamData, streamData + pulledSize, streamLength);
}

Stream::Stream() :streamLength(0) { memset(streamData, 0, BUFFER_SIZE); }

Stream::~Stream() {}

char* Stream::GetStream()
{
	CriticalSectionSync sync(cs);

	return streamData;
}

std::size_t Stream::GetStreamLength()
{
	CriticalSectionSync sync(cs);

	return streamLength;
}

void Stream::SetStreamLength(std::size_t length)
{
	CriticalSectionSync sync(cs);

	streamLength = length;
};

void Stream::RemoveSendedLength(std::size_t length)
{
	CriticalSectionSync sync(cs);

	streamLength -= length;
	//PullRemainedData(length);
}

void Stream::ResetStream()
{
	CriticalSectionSync sync(cs);

	memset(streamData, 0, 4096);
	streamLength = 0;
}

std::size_t Stream::GetFreeSize() { return 4096 - streamLength; }