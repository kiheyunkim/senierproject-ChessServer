#include "stdafx.h"
#include "GameRoom.h"
#include "NetworkSession.h"
#include "GameRoomManager.h"

bool GameRoomManager::IncreaseRoomCount()
{
	CriticalSectionSync sync(cs);

	if (currentRoomSize + 1 > maxRoomSize) return false;	///Exceed

	currentRoomSize += 1;
	return true;
}

bool GameRoomManager::DecreaseRoomCount()
{
	CriticalSectionSync sync(cs);

	if (currentRoomSize - 1 < 0) return false;

	currentRoomSize -= 1;
	return true;
}

GameRoomManager::GameRoomManager(std::size_t allocSize) :maxRoomSize(allocSize), currentRoomSize(0) {}

GameRoomManager::~GameRoomManager() {}

void GameRoomManager::PrepareRooms()
{
	for (std::size_t i = 0; i < maxRoomSize; i++)
		roomVector[i] = new GameRoom(static_cast<int>(i));
		//roomVector.PushBack(new GameRoom(static_cast<int>(i)));
}

void GameRoomManager::CleanUpRooms()
{
	for (std::size_t i = 0; i < maxRoomSize; i++)
	{
		roomVector[i]->EndGameRoom();
		delete roomVector[i];
		//roomVector.Back()->EndGameRoom();
		//delete roomVector.Back();
		//roomVector.PopBack();
	}
}

bool GameRoomManager::JoinRoomWithNumber(std::size_t roomNumber, NetworkSession* player)
{
	CriticalSectionSync sync(cs);

	if (roomNumber >= currentRoomSize || roomNumber < 0) return false;
	if (player == nullptr) return false;

	if (!roomVector[roomNumber]->JoinRoom(player)) return false;

	return true;
}

GameRoom* GameRoomManager::GetGameRoomWithNumber(std::size_t roomNumber)
{
	CriticalSectionSync sync(cs);

	if (roomNumber > currentRoomSize) return nullptr;
	return roomVector[roomNumber];
}

bool GameRoomManager::LeaveRoomRequest(NetworkSession* player)
{
	CriticalSectionSync sync(cs);

	if (player == nullptr) return false;
	if (player->GetRoomNumber() > currentRoomSize || player->GetRoomNumber() < 0) return false;

	if (!roomVector[player->GetRoomNumber()]->LeaveRoom(player)) return false;

	return true;
}

bool GameRoomManager::CreateRoom(NetworkSession* player)
{
	CriticalSectionSync sync(cs);

	if (player == nullptr) return false;
	if (!IncreaseRoomCount()) return false;

	return true;
}

bool GameRoomManager::QuickJoin(NetworkSession* player)
{
	CriticalSectionSync sync(cs);

	if (player == nullptr) return false;
	if (player->GetRoomNumber() != -1) return false;

	std::size_t roomSize = currentRoomSize;
	for (std::size_t i = 0; i < roomSize; i++)
	{
		if (!roomVector[i]->isRoomFull())
		{
			roomVector[i]->JoinRoom(player);
			return true;
		}
	}

	if (!IncreaseRoomCount()) return false;

	roomVector[currentRoomSize - 1]->JoinRoom(player);

	return true;
}

int GameRoomManager::GetCurrentRoomSize() { return static_cast<int>(currentRoomSize); }