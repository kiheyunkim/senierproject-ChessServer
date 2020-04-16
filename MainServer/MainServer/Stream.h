#ifndef _STREAM_H_
#define _STREAM_H_
#include "CriticalSection.h"

class Packet;
class Stream
{
private:
	friend class Packet;

private:
	static std::size_t headerSize;

private:
	CriticalSection cs;

private:
	char streamData[4096];
	std::size_t streamLength;

private:
	void PullRemainedData(std::size_t pulledSize);

public:
	Stream();
	~Stream();

public:
	char* GetStream();
	std::size_t GetStreamLength();
	void SetStreamLength(std::size_t length);
	void RemoveSendedLength(std::size_t length);

public:
	void ResetStream();
	std::size_t GetFreeSize();
};

#endif // !_STREAM_H_
