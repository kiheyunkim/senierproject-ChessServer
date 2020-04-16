#include"stdafx.h"
#include "Protocol.h"
#include"ChessObject.h"
#include"Knight.h"

Knight::Knight(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType)
	:ChessObject(pos, teamType, pieceType)
{
	if (teamType == TEAM_TYPE::ENEMY)
		setImage(_T("Knight.png"), _T("Knight.png"));
	if (teamType == TEAM_TYPE::ALLY)
		setImage(_T("Knight.png"), _T("Knight.png"));
}

Knight::~Knight() {} 

void Knight::SetMovableArea(std::vector<int>& movableArea)
{
	movableArea.push_back(GetPos());

	///Forward
	if ((GetPos() / 8) + 1 > 2)
	{
		if (((GetPos() % 8) + 1) > 1)
			movableArea.push_back(GetPos() - 16 - 1);
		if (((GetPos() % 8) + 1) < 8)
			movableArea.push_back(GetPos() - 16 + 1);
	}

	///Backward
	if ((GetPos() / 8) + 1 < 7)
	{
		if (((GetPos() % 8) + 1) > 1)
			movableArea.push_back(GetPos() + 16 - 1);
		if (((GetPos() % 8) + 1) < 8)
			movableArea.push_back(GetPos() + 16 + 1);
	}

	///Left
	if (((GetPos() % 8) + 1) > 2)
	{
		if ((GetPos() / 8) + 1 > 1)
			movableArea.push_back(GetPos() - 2 - 8);
		if ((GetPos() / 8) + 1 < 8)
			movableArea.push_back(GetPos() - 2 + 8);
	}
	
	///Right
	if (((GetPos() % 8) + 1) < 7)
	{
		if ((GetPos() / 8) + 1 > 1)
		movableArea.push_back(GetPos() + 2 - 8);
		if ((GetPos() / 8) + 1 < 8)
		movableArea.push_back(GetPos() + 2 + 8);
	}
}