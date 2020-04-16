#include "stdafx.h"
#include "Protocol.h"
#include "ChessObject.h"
#include "King.h"

King::King(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType)
	:ChessObject(pos,teamType,pieceType)
{
	if (teamType == TEAM_TYPE::ALLY)
		setImage(_T("King.png"), _T("King.png"));
	if (teamType == TEAM_TYPE::ENEMY)
		setImage(_T("King.png"), _T("King.png"));
}

King::~King() {}

void King::SetMovableArea(std::vector<int>& movableArea)
{
	bool checkArr[9]{ true,true,true,true,true,true,true,true,true };

	///top edge
	if ((GetPos() / 8) + 1 == 1)
	{
		for (std::size_t i = 0; i < 3; i++)
			checkArr[i] = false;
	}
	///bottom edge
	if ((GetPos() / 8) + 1 == 8)
	{
		for (std::size_t i = 6; i < 9; i++)
			checkArr[i] = false;
	}
	///left edge
	if ((GetPos() % 8) + 1 == 1)
	{
		for (std::size_t i = 0; i < 3; i++)
			checkArr[i * 3] = false;
	}
	///right edge
	if ((GetPos() % 8) + 1 == 8)
	{
		for (std::size_t i = 0; i < 3; i++)
			checkArr[(i * 3) + 2] = false;
	}

	///Draw
	for (std::size_t i = 0; i < 3; i++)			///0~2
		if (checkArr[i]) movableArea.push_back(GetPos() - 8 - 1 + static_cast<int>((i)));
	for (std::size_t i = 0; i < 3; i++)			///3~5
		if (checkArr[3 + i]) movableArea.push_back(GetPos() - 1 + static_cast<int>((i)));
	for (std::size_t i = 0; i < 3; i++)			///6~8
		if (checkArr[6 + i]) movableArea.push_back(GetPos() + 8 - 1 + static_cast<int>((i)));
}