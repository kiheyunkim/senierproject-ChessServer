class PacketLoginContent
{
private:
	std::string userID;
	std::string userPasswd;

public:
	PacketLoginContent();
	PacketLoginContent(const char* id, const char* pw);
	~PacketLoginContent();
	const char* GetID() const;
	const char* GetPW() const;
	void SetID(const char* id);
	void SetPW(const char* pw);

	friend std::ostream& operator<<(std::ostream& out, PacketLoginContent& content)
	{
		out << content.userID << " " << content.userPasswd;
		return out;
	}

	friend std::istream& operator>>(std::istream& in, PacketLoginContent& content)
	{
		in >> content.userID >> content.userPasswd;
		return in;
	}
};

class PacketJoinContent
{
private:
	std::string userID;
	std::string userPasswd;
	std::string userNick;

public:
	PacketJoinContent();
	PacketJoinContent(const char* id, const char* pw, const char* nick);
	~PacketJoinContent();
	const char* GetID() const;
	const char* GetPW() const;
	const char* GetNick() const;
	void SetID(const char* id);
	void SetPW(const char* pw);
	void SetNick(const char* nick);

	friend std::ostream& operator<<(std::ostream& out, PacketJoinContent& content)
	{
		out << content.userID << " " << content.userPasswd << " " << content.userNick;
		return out;
	}

	friend std::istream& operator>>(std::istream& in, PacketJoinContent& content)
	{
		in >> content.userID >> content.userPasswd >> content.userNick;
		return in;
	}
};

class PacketPeiceContent
{
private:
	int requestedGridNum;
	REQUESTED_PIECE_TYPE requestedPieceType;

public:
	PacketPeiceContent();
	PacketPeiceContent(int number,REQUESTED_PIECE_TYPE type);
	~PacketPeiceContent();
	int GetRequestedGridNumber() const;
	REQUESTED_PIECE_TYPE GetRequestedPieceType() const;
	void SetRequestedGridNumber(int number);
	void SetRequestedPieceType(REQUESTED_PIECE_TYPE type);

	friend std::ostream& operator<<(std::ostream& out, PacketPeiceContent& content)
	{
		out << content.requestedGridNum << " " << static_cast<int>(content.requestedPieceType);
		return out;
	}

	friend std::istream& operator>>(std::istream& in, PacketPeiceContent& content)
	{
		int type{};
		in >> content.requestedGridNum >> type;
		content.requestedPieceType = static_cast<REQUESTED_PIECE_TYPE>(type);
		return in;
	}
};

class PacketMessageContent
{
private:
	std::size_t length;
	std::string message;

public:
	PacketMessageContent();
	PacketMessageContent(std::size_t len, const char* msg);
	~PacketMessageContent();
	std::size_t	 GetMessageLength() const;
	const char* GetChatMessage() const;
	void SetLength(std::size_t len);
	void SetMessage(const char* msg);

	friend std::ostream& operator<<(std::ostream& out, PacketMessageContent& content)
	{
		out << content.length << " " << content.message.c_str();
		return out;
	}

	friend std::istream& operator>>(std::istream& in, PacketMessageContent& content)
	{
		char buffer[512] = { 0, };
		in >> content.length;
		in.seekg(1, std::ios::cur);
		in.getline(buffer, content.length);
		content.message.assign(buffer);
		return in;
	}
};