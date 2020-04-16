#ifndef _STREAM_H_
#define _STREAM_H_

class Packet;
class Stream
{
private:
	static std::size_t headerSize;

private:
	char streamData[4096];
	std::size_t streamLength;

private:
	void PullRemainedData(std::size_t pulledSize);

public:
	Stream();
	Stream(const Packet& packet);
	~Stream();

public:
	char* GetStream();
	std::size_t GetStreamLength();
	void SetStreamLength(std::size_t length);
	void RemoveSendedLength(std::size_t length);

public:
	bool GetDataFromPacket(Packet* packet);
	bool SetDataToPacket(Packet* packet, std::size_t length) const;
	void ResetStream();
	std::size_t GetFreeSize();
};

#endif // !_STREAM_H_
