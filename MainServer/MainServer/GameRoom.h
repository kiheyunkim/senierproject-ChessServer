#ifndef _GAMEROOM_H_
#define _GAMEROOM_H_
#include "CriticalSection.h"

class NetworkSession;
class GameRoom
{
private:
	bool gameStart;
	bool gameStartReady;
	int roomNumber;
	int count;

	NetworkSession* playerAlpha;
	NetworkSession* playerBeta;

	bool alphaReadyStatus;
	bool betaReadyStatus;

private:
	CriticalSection cs;

private:
	std::map<PIECE_TYPE, std::size_t> alphaPoses;
	std::map<PIECE_TYPE, std::size_t> betaPoses;

private:
	inline int convertRelativePos(int pos);
	bool deleteIfOpponentThere(NetworkSession* player, int pos);

public:
	GameRoom(int roomNumber);
	~GameRoom();

	bool IsAlphaReady();
	bool IsBetaReady();

	bool StartGameRoom(NetworkSession* player);
	bool EndGameRoom();

	bool GameReady(NetworkSession* player);
	bool SetStartReadyState();
	bool GetIsReadyToStart();
	bool isRoomFull();
	bool isPlaying();

	bool JoinRoom(NetworkSession* player);
	bool LeaveRoom(NetworkSession* player);

	bool MovePiece(NetworkSession* player, PIECE_TYPE type, int currentPos, int movePos);
	
	bool CleanUpForDisconnection(NetworkSession* disConnectedPlayer);
	int GetRoomCount();

	void ProgressForWinAndLose(NetworkSession* player);
};

#endif // !_GAMEROOM_H_
