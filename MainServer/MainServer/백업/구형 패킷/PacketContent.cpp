#include"stdafx.h"
#include"PacketStream.h"
#include"Protocol.h"
#include <sstream>
#include"PacketContent.h"

///Login Packet
PacketLoginContent::PacketLoginContent() :userID(""), userPasswd("") {}

PacketLoginContent::PacketLoginContent(const char* id, const char* pw) :userID(id), userPasswd(pw) {}

PacketLoginContent::~PacketLoginContent() {}

const char* PacketLoginContent::GetID() const { return userID.c_str(); }

const char* PacketLoginContent::GetPW() const { return userPasswd.c_str(); }

void PacketLoginContent::SetID(const char* id) { userID = id; }

void PacketLoginContent::SetPW(const char* pw) { userPasswd = pw; }


///Join Packet
PacketJoinContent::PacketJoinContent() :userID(""), userPasswd(""), userNick("") {}

PacketJoinContent::PacketJoinContent(const char* id, const char* pw, const char* nick) :userID(id), userPasswd(pw), userNick(nick) {}

PacketJoinContent::~PacketJoinContent() {}

const char* PacketJoinContent::GetID() const { return userID.c_str(); }

const char* PacketJoinContent::GetPW() const { return userPasswd.c_str(); }

const char* PacketJoinContent::GetNick() const { return userNick.c_str(); }

void PacketJoinContent::SetID(const char* id) { userID = id; }

void PacketJoinContent::SetPW(const char* pw) { userPasswd = pw; }

void PacketJoinContent::SetNick(const char* nick) { userNick = nick; }


///Piece Packet
PacketPeiceContent::PacketPeiceContent() :requestedGridNum(0), requestedPieceType(REQUESTED_PIECE_TYPE::PIECE_TYPE_HEAD) {}

PacketPeiceContent::PacketPeiceContent(int number, REQUESTED_PIECE_TYPE type) : requestedGridNum(number), requestedPieceType(type) {}

PacketPeiceContent::~PacketPeiceContent() {}

int PacketPeiceContent::GetRequestedGridNumber() const { return requestedGridNum; }

REQUESTED_PIECE_TYPE PacketPeiceContent::GetRequestedPieceType() const { return requestedPieceType; }

void PacketPeiceContent::SetRequestedGridNumber(int number) { requestedGridNum = number; }

void PacketPeiceContent::SetRequestedPieceType(REQUESTED_PIECE_TYPE type) { requestedPieceType = type; }


///Message Packet
PacketMessageContent::PacketMessageContent() :length(0), message("") {}

PacketMessageContent::PacketMessageContent(std::size_t len, const char* msg) : length(len), message(msg) {}

PacketMessageContent::~PacketMessageContent() {}

std::size_t PacketMessageContent::GetMessageLength() const { return length; }

const char* PacketMessageContent::GetChatMessage() const { return message.c_str(); }

void PacketMessageContent::SetLength(std::size_t len) { length = len; }

void PacketMessageContent::SetMessage(const char* msg) { message = msg; }
