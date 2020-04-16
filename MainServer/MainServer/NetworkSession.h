#ifndef _NETWORKSESSION_H_
#define _NETWORKSESSION_H_

#include "Stream.h"
#include "CriticalSection.h"
#include "MemoryPool.h"


class NetworkSession;
class SessionManager;

enum class SessionType
{
	SESSION_TYPE_HEADER,
	SQL_SERVER,
	LOGIN_SERVER
};

enum class IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,
	IO_ACCEPT,
	IO_CONNECT,
	IO_DISCONNECT
};

enum class DisconnectReason
{
	DR_NONE,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_IO_REQUEST_ERROR,
	DR_COMPLETION_ERROR,
};

struct OverlappedIOContext
{
	OverlappedIOContext(NetworkSession* session, IOType ioType);
	OVERLAPPED			overlapped;
	NetworkSession*		sessionObject;
	IOType				ioType;
	WSABUF				wsaBuf;
};

struct OverlappedSendContext : public OverlappedIOContext
{
	OverlappedSendContext(NetworkSession* session) : OverlappedIOContext(session, IOType::IO_SEND) {}
};

struct OverlappedRecvContext : public OverlappedIOContext
{
	OverlappedRecvContext(NetworkSession* session) : OverlappedIOContext(session, IOType::IO_RECV) {}
};

struct OverlappedDisconnectContext : public OverlappedIOContext
{
	OverlappedDisconnectContext(NetworkSession* session, DisconnectReason dr)
		: OverlappedIOContext(session, IOType::IO_DISCONNECT), disconnectReason(dr) {}
	DisconnectReason disconnectReason;
};

struct OverlappedAcceptContext : public OverlappedIOContext
{
	OverlappedAcceptContext(NetworkSession* owner) : OverlappedIOContext(owner, IOType::IO_ACCEPT) {}
};

struct OverlappedConnectContext : public OverlappedIOContext
{
	OverlappedConnectContext(NetworkSession* owner) : OverlappedIOContext(owner, IOType::IO_CONNECT) {}
};

void DeleteIoContext(OverlappedIOContext* context);


class NetworkSession
{
private:
	friend class SessionManager;

private:
	SessionType sessionType;

private:
	SOCKET			sessionSocket;
	SOCKADDR_IN		sessionAddress;

	volatile long	refCount;
	volatile long	connected;

	Stream recvedStream;
	Stream sendStream;

private:
	CriticalSection cs;


private:
	int gameRoomNumber;
	std::string identifyStr;
	bool readyStatus;

public:
	NetworkSession();
	~NetworkSession();

	void	ResetSession();
	bool	IsConnected() const { return !!connected; }

	bool	PostAccept();
	void	AcceptCompletion();

	bool	Connect(const char* ipAddr, unsigned int port, SessionType type);
	void	ConnectCompletion();
	bool	ConvertToConnectionSession();

	bool	PostRecv();
	void	RecvCompletion(DWORD transferred);

	bool	PostSend();
	void	SendCompletion(DWORD transferred);

	void	DisconnectRequest(DisconnectReason dr);
	void	DisconnectCompletion(DisconnectReason dr);

	void	AddRef();
	void	ReleaseRef();

	inline	void SetSocket(SOCKET sock) { sessionSocket = sock; }
	inline	SOCKET GetSocket() const { return sessionSocket; }

	bool	AddStream(const Stream& stream);
	
	unsigned short GetPort();

public:
	int GetRoomNumber();
	void SetRoomNumber(int number);

	void SetName(const char* str);
	void SetReadyState(bool state);

	void ProcessForDisconnect();

};

#endif // !_NETWORKSESSION_H_





