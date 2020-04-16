#ifndef _GAMEROOMMANAGER_H_
#define _GAMEROOMMANAGER_H_
#include "CriticalSection.h"

class GameRoom;
class NetworkSession;
class GameRoomManager
{
private:
	const std::size_t		maxRoomSize;
	std::size_t				currentRoomSize;
	//std::vector<GameRoom*>	roomVector;
	GameRoom*					roomVector[MAX_ROOM];

private:
	CriticalSection cs;

private:
	inline bool IncreaseRoomCount();
	inline bool DecreaseRoomCount();

public:
	GameRoomManager(std::size_t allocSize);
	~GameRoomManager();
	void PrepareRooms();
	void CleanUpRooms();

public:
	bool JoinRoomWithNumber(std::size_t roomNumber, NetworkSession* player);
	GameRoom* GetGameRoomWithNumber(std::size_t roomNumber);
	bool LeaveRoomRequest(NetworkSession* player);
	bool CreateRoom(NetworkSession* player);
	bool QuickJoin(NetworkSession* player);
	int GetCurrentRoomSize();
};

#endif