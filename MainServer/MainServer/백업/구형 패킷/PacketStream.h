#ifndef _PACKETSTREAM_H_
#define _PACKETSTREAM_H_
#include<sstream>
class PacketStream
{
private:
	PACKET_TYPE header;
	std::stringstream contents;

public:
	PacketStream();
	~PacketStream();
	void SetHeader(PACKET_TYPE header);
	PACKET_TYPE GetHeader();
	void SetContent(const char* content);
	std::string GetContent();

	friend std::ostream& operator<<(std::ostream& out, PacketStream& packet)
	{
		out << static_cast<int>(packet.header) << " " << packet.contents.str().c_str();
		return out;
	}

	friend std::istream& operator>>(std::istream& in, PacketStream& packet)
	{
		char buffer[512] = { 0, };
		int header{};

		in >> header;									///Extract header
		in.getline(buffer, 4000);						///Extract contents
		packet.contents << buffer;
		packet.contents.str().push_back('\0');
		packet.header = static_cast<PACKET_TYPE>(header);
		return in;
	}
};

#endif // !_PACKETSTREAM_H_
