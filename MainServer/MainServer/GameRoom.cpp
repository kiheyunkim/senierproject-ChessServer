#include "stdafx.h"
#include "NetworkSession.h"
#include "Packet.h"
#include "Stream.h"
#include "GameRoom.h"

int GameRoom::convertRelativePos(int pos)
{
	return 63 - pos;
}

bool GameRoom::deleteIfOpponentThere(NetworkSession* player, int pos)
{
	CriticalSectionSync sync(cs);

	if (player == nullptr) return false;

	if (player == playerAlpha)
	{
		for (std::map<PIECE_TYPE, std::size_t>::iterator iter = betaPoses.begin(); iter != betaPoses.end(); iter++)
		{
			if (iter->second == convertRelativePos(pos))
			{
				if (iter->first == PIECE_TYPE::KING)
					ProgressForWinAndLose(player);
				betaPoses.erase(iter);

				return true;
			}
		}
	}

	if (player == playerBeta)
	{
		for (std::map<PIECE_TYPE, std::size_t>::iterator iter = alphaPoses.begin(); iter != alphaPoses.end(); iter++)
		{
			if (iter->second == convertRelativePos(pos))
			{
				if (iter->first == PIECE_TYPE::KING)
					ProgressForWinAndLose(player);
				alphaPoses.erase(iter);

				return true;
			}
		}
	}

	return false;
}

GameRoom::GameRoom(int roomNumber)
	:gameStart(false), roomNumber(roomNumber), playerAlpha(nullptr), playerBeta(nullptr),
	alphaReadyStatus(false), betaReadyStatus(false), count(0) {}


GameRoom::~GameRoom() {}

bool GameRoom::IsAlphaReady() { return alphaReadyStatus; }

bool GameRoom::IsBetaReady() { return betaReadyStatus; }

bool GameRoom::StartGameRoom(NetworkSession* player)
{
	CriticalSectionSync sync(cs);

	if (player == nullptr) return false;
	if (playerAlpha == nullptr || playerBeta == nullptr) return false;
	if (playerAlpha->GetRoomNumber() == -1 || playerBeta->GetRoomNumber() == -1) return false;
	if (!betaReadyStatus) return false;

	gameStart = true;

	alphaPoses.clear();
	betaPoses.clear();

	///Dispose all pieces
	for (std::size_t i = 0; i < 8; i++)
	{
		alphaPoses.insert(std::make_pair(static_cast<PIECE_TYPE>(static_cast<int>(PIECE_TYPE::PAWN1) + i), 48 + i));
		alphaPoses.insert(std::make_pair(static_cast<PIECE_TYPE>(static_cast<int>(PIECE_TYPE::L_ROOK) + i), 56 + i));
	}

	for (std::size_t i = 0; i < 8; i++)
	{
		betaPoses.insert(std::make_pair(static_cast<PIECE_TYPE>(static_cast<int>(PIECE_TYPE::PAWN1) + i), 48 + i));
		betaPoses.insert(std::make_pair(static_cast<PIECE_TYPE>(static_cast<int>(PIECE_TYPE::L_ROOK) + i), 56 + i));
	}

	//Packet packetStartFirst(PACKET_TYPE::START_REQUEST_FIRST);
	//Stream streamAlpha(packetStartFirst);
	//Packet packetStart(PACKET_TYPE::START_REQUEST);
	//Stream streamBeta(packetStart);

	//playerAlpha->AddStream(streamAlpha);
	//playerBeta->AddStream(streamBeta);

	if (playerAlpha == player)
		playerBeta->PostSend();
	else
		playerAlpha->PostSend();
	
	logManager->WriteLog("[DEBUG] Room %d Success to Game Start", roomNumber);

	return true;
}

bool GameRoom::EndGameRoom()
{
	CriticalSectionSync sync(cs);

	alphaPoses.clear();
	betaPoses.clear();
	
	gameStart = false;

	return true;
}

bool GameRoom::GameReady(NetworkSession* player)
{
	CriticalSectionSync sync(cs);

	if (player == playerAlpha)
	{
		alphaReadyStatus = alphaReadyStatus ? false : true;
		return true;
	}

	if (player == playerBeta)
	{
		betaReadyStatus = betaReadyStatus ? false : true;
		return true;
	}

	return false;
}

bool GameRoom::SetStartReadyState()
{
	CriticalSectionSync sync(cs);

	gameStartReady = alphaReadyStatus && betaReadyStatus;
	return true;
}

bool GameRoom::GetIsReadyToStart()
{
	return gameStartReady;
}

bool GameRoom::JoinRoom(NetworkSession* player)
{
	CriticalSectionSync sync(cs);

	if (playerAlpha != nullptr && playerBeta != nullptr) return false;
	if (player == nullptr) return false;

	if (playerAlpha == nullptr)
	{
		playerAlpha = player;
		player->SetRoomNumber(roomNumber);
		count++;
		return true;
	}

	if (playerBeta == nullptr)
	{
		playerBeta = player;
		player->SetRoomNumber(roomNumber);
		count++;
		return true;
	}

	return false;
}


bool GameRoom::LeaveRoom(NetworkSession* player)
{
	CriticalSectionSync sync(cs);

	if (playerAlpha == player)
	{
		playerAlpha = nullptr;
		player->SetRoomNumber(-1);
		count--;
		return true;
	}

	if (playerBeta == player)
	{
		playerBeta = nullptr;
		player->SetRoomNumber(-1);
		count--;
		return true;
	}

	return false;
}

bool GameRoom::isRoomFull() 
{
	CriticalSectionSync sync(cs);

	return playerAlpha != nullptr && playerBeta != nullptr; 
}

bool GameRoom::isPlaying() { return gameStart; }

bool GameRoom::MovePiece(NetworkSession* player, PIECE_TYPE type, int currentPos, int movePos)
{
	CriticalSectionSync sync(cs);

	if (playerAlpha == nullptr || playerBeta == nullptr) return false;
	if (player == nullptr) return false;
	if (currentPos < 0 && currentPos > 63) return false;
	if (movePos < 0 && movePos > 63) return false;

	if (playerAlpha == player)
	{
		logManager->WriteLog("RoomNumber:%d ,player : %p, PIECE_TYPE : %d, requestedPos : %d, currentPos : %d", roomNumber, player, static_cast<int>(type), movePos, currentPos);
		std::map<PIECE_TYPE, std::size_t>::iterator iter = alphaPoses.find(type);
		
		if (iter->second != currentPos)	return false;
		iter->second = movePos;

		//Packet packet(PACKET_TYPE::PIECE_MOVING);
		//packet.SetMovingPeice(TEAM_TYPE::ENEMY, type, convertRelativePos(movePos));
		//playerBeta->AddStream(packet);
		playerBeta->PostSend();

		deleteIfOpponentThere(player, movePos);

		return true;
	}

	if (playerBeta == player)
	{
		logManager->WriteLog("RoomNumber:%d ,player : %p, PIECE_TYPE : %d, requestedPos : %d, currentPos : %d", roomNumber, player, static_cast<int>(type), movePos, currentPos);
		std::map<PIECE_TYPE, std::size_t>::iterator iter = betaPoses.find(type);

		if (iter->second != currentPos) return false;
		iter->second = movePos;

		//Packet packet(PACKET_TYPE::PIECE_MOVING);
		//packet.SetMovingPeice(TEAM_TYPE::ENEMY, type, convertRelativePos(movePos));
		//playerAlpha->AddStream(packet);
		playerAlpha->PostSend();

		deleteIfOpponentThere(player, movePos);

		return true;
	}

	return false;
}

bool GameRoom::CleanUpForDisconnection(NetworkSession* disConnectedPlayer)
{
	CriticalSectionSync sync(cs);

	if (disConnectedPlayer == nullptr) return false;

	//Packet packet(PACKET_TYPE::OPPONENT_DISCONNECTED);
	//Stream stream(packet);

	//if (disConnectedPlayer != playerAlpha)
	//{
	//	playerBeta = nullptr;
	//	if (playerAlpha != nullptr)
	//	{
	//		playerAlpha->SetReadyState(false);
	//		playerAlpha->AddStream(stream);
	//		playerAlpha->PostSend();
	//	}
	//}
	//else
	//{
	//	playerAlpha = nullptr;
	//	if (playerBeta != nullptr)
	//	{
	//		playerBeta->SetReadyState(false);
	//		playerBeta->AddStream(stream);
	//		playerBeta->PostSend();
	//	}
	//}
	//
	//betaReadyStatus = false;
	//alphaReadyStatus = false;
	//gameStart = false;
	//gameStartReady = false;
	//count -= 1;
	//
	//return true;
}

int GameRoom::GetRoomCount() { return count; }

void GameRoom::ProgressForWinAndLose(NetworkSession* player)
{
	//CriticalSectionSync sync(cs);
	//
	//Packet packetWin(PACKET_TYPE::GAME_WIN);
	//Packet packetLose(PACKET_TYPE::GAME_LOSE);
	//Stream winStream(packetWin);
	//Stream loseStream(packetLose);
	//
	//if (player == playerAlpha)
	//{
	//	playerAlpha->AddStream(winStream);
	//	playerBeta->AddStream(loseStream);
	//	playerBeta->PostSend();
	//}
	//else if (player == playerBeta)
	//{
	//	playerAlpha->AddStream(loseStream);
	//	playerBeta->AddStream(winStream);
	//	playerAlpha->PostSend();
	//}
	//
	//playerAlpha->SetReadyState(false);
	//playerBeta->SetReadyState(false);
	//betaReadyStatus = false;
	//alphaReadyStatus = false;
	//gameStart = false;
	//gameStartReady = false;
}