#include "stdafx.h"
#include "Protocol.h"
#include "ChessObject.h"
#include "Queen.h"

Queen::Queen(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType)
	:ChessObject(pos, teamType, pieceType)
{
	if (teamType == TEAM_TYPE::ALLY)
		setImage(_T("Queen.png"), _T("Queen.png"));
	if (teamType == TEAM_TYPE::ENEMY)
		setImage(_T("Queen.png"), _T("Queen.png"));
}

Queen::~Queen() {}

void Queen::SetMovableArea(std::vector<int>& movableArea)
{
	movableArea.push_back(GetPos());
}