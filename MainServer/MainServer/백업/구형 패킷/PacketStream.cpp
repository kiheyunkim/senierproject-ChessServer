#include"stdafx.h"
#include <sstream>
#include"PacketStream.h"

PacketStream::PacketStream() :header(PACKET_TYPE::PACKET_TYPE_HEAD) {}

PacketStream::~PacketStream() {}

void PacketStream::SetHeader(PACKET_TYPE type) { header = type; }

PACKET_TYPE PacketStream::GetHeader() { return header; }

void PacketStream::SetContent(const char* content) { contents << content; }

std::string PacketStream::GetContent() { return contents.str(); }