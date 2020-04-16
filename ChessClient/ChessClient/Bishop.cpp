#include "stdafx.h"
#include "Protocol.h"
#include "ChessObject.h"
#include "Bishop.h"

Bishop::Bishop(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType)
	:ChessObject(pos, teamType, pieceType)
{
	if (teamType == TEAM_TYPE::ENEMY)
		setImage(_T("Bishop.png"), _T("Bishop.png"));
	if (teamType == TEAM_TYPE::ALLY)
		setImage(_T("Bishop.png"), _T("Bishop.png"));
}

Bishop::~Bishop() {}

void Bishop::SetMovableArea(std::vector<int>& movableArea)
{
	movableArea.push_back(GetPos());
}