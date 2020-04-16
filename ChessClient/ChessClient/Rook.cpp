#include"stdafx.h"
#include "Protocol.h"
#include"ChessObject.h"
#include"Rook.h"

Rook::Rook(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType)
	:ChessObject(pos, teamType, pieceType)
{
	if (teamType == TEAM_TYPE::ENEMY)
		setImage(_T("Rook.png"), _T("Rook.png"));
	else if (teamType == TEAM_TYPE::ALLY)
		setImage(_T("Rook.png"), _T("Rook.png"));
}

Rook::~Rook() {}

void Rook::SetMovableArea(std::vector<int>& movableArea)
{
	movableArea.push_back(GetPos());
}
