#include "stdafx.h"
#include "Protocol.h"
#include "Pawn.h"

Pawn::Pawn(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType)
	:ChessObject(pos, teamType, pieceType)
{
	if (teamType == TEAM_TYPE::ENEMY)
		setImage(_T("pawn1.png"), _T("pawn1.png"));
	else if (teamType == TEAM_TYPE::ALLY)
		setImage(_T("pawn1.png"), _T("pawn1.png"));
}

Pawn::~Pawn() {}

void Pawn::SetMovableArea(std::vector<int>& movableArea)
{
	int currentPos = GetPos();

	movableArea.push_back(currentPos);	///Current Pos
	movableArea.push_back(currentPos - 8 );
	if (isFirstMove) 
		movableArea.push_back(currentPos - 16);
}