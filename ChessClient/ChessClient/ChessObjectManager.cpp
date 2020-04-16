#include "stdafx.h"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "King.h"
#include "Queen.h"
#include "ChessObject.h"
#include "ChessObjectManager.h"

bool ChessObjectManager::deleteObjectFromGrid(TEAM_TYPE teamType, int gridNum)
{
	for (std::vector<ChessObject*>::const_iterator iter = chessObjects.cbegin(); iter != chessObjects.cend();iter++)
	{
		if ((*iter)->GetPos() == gridNum&&(*iter)->GetTeamType()==teamType)
		{
			ChessObject* temp = *iter;
			chessObjects.erase(iter);
			delete temp;
			return true;
		}
	}

	return false;
}

std::pair<int, int> ChessObjectManager::GetPosFromGrid(int gridNum) const
{
	int x{ 40 + (gridNum % 8 * 80) };
	int y{ 40 + (gridNum / 8 * 80) };

	return std::make_pair(x, y);
}

int ChessObjectManager::GetGridFromPos(std::pair<int, int> pair) const
{
	return (pair.first / 80) + ((pair.second / 80) * 8);
}

void ChessObjectManager::AdjustMovableAreaPawn()
{
	if (isThereEnemy(clickedTarget->GetPos() - 8) || isThereAlly(clickedTarget->GetPos() - 8))			/// forward somethig is exist
		movableArea.clear();
	else if (isThereEnemy(clickedTarget->GetPos() - 16) || isThereAlly(clickedTarget->GetPos() - 16))	/// +2 forward somthing is exist
	{
		movableArea.clear();
		movableArea.push_back(clickedTarget->GetPos() - 8);
	}

	movableArea.push_back(clickedTarget->GetPos());

	///Pawn Attack Arrange
	if (clickedTarget->GetPos() % 8 != 0 && isThereEnemy(clickedTarget->GetPos() - 8 - 1))				///left upper
	{
		movableArea.push_back(clickedTarget->GetPos() - 8 - 1);
		attackArea.push_back(clickedTarget->GetPos() - 8 - 1);
	}
	if (clickedTarget->GetPos() % 8 != 7 && isThereEnemy(clickedTarget->GetPos() - 8 + 1))				///right upper
	{
		movableArea.push_back(clickedTarget->GetPos() - 8 + 1);
		attackArea.push_back(clickedTarget->GetPos() - 8 + 1);
	}
}

void ChessObjectManager::AdjustMovableAreaNormal()
{
	for (std::vector<int>::const_iterator iter = movableArea.cbegin(); iter != movableArea.cend();)
	{
		if (clickedTarget->GetPos() == *iter)		///Except itSelf
		{
			iter++;
			continue;
		}
		if (isThereAlly(*iter))						///Cannot Move on Ally's Position
			iter = movableArea.erase(iter);
		else
			iter++;
	}

	for (std::vector<int>::const_iterator iter = movableArea.cbegin(); iter != movableArea.cend(); iter++)
	{
		if (isThereEnemy(*iter))
			attackArea.push_back(*iter);
	}
}

void ChessObjectManager::AdjustMovableAreaBishop()
{
	bool northWest{ false }, northEast{ false }, southWest{ false }, southEast{ false };

	if (clickedTarget->GetPos() / 8 != 0 && clickedTarget->GetPos() % 8 != 0) northWest = true;
	if (clickedTarget->GetPos() / 8 != 0 && clickedTarget->GetPos() % 8 != 7) northEast = true;
	if (clickedTarget->GetPos() / 8 != 7 && clickedTarget->GetPos() % 8 != 0) southWest = true;
	if (clickedTarget->GetPos() / 8 != 7 && clickedTarget->GetPos() % 8 != 7) southEast = true;

	///North West
	for (std::size_t i = 1; northWest != false; ++i)
	{
		if (isThereAlly(static_cast<int>(clickedTarget->GetPos() - (8 * i) - i))) break;
		movableArea.push_back(static_cast<int>(clickedTarget->GetPos() - (8 * i) - i));
		if (isThereEnemy(static_cast<int>(clickedTarget->GetPos() - (8 * i) - i)))
		{
			attackArea.push_back(static_cast<int>(clickedTarget->GetPos() - (8 * i) - i));
			break;
		}
		if (clickedTarget->IsInEdge(static_cast<int>(clickedTarget->GetPos() - (8 * i) - i))) break;
	}

	///North East
	for (std::size_t i = 1; northEast != false; ++i)
	{
		if (isThereAlly(static_cast<int>(clickedTarget->GetPos() - (8 * i) + i))) break;
		movableArea.push_back(static_cast<int>(clickedTarget->GetPos() - (8 * i) + i));
		if (isThereEnemy(static_cast<int>(clickedTarget->GetPos() - (8 * i) + i)))
		{
			attackArea.push_back(static_cast<int>(clickedTarget->GetPos() - (8 * i) + i));
			break;
		}
		if (clickedTarget->IsInEdge(static_cast<int>(clickedTarget->GetPos() - (8 * i) + i))) break;
	}

	///South West
	for (std::size_t i = 1; southWest != false; ++i)
	{
		if (isThereAlly(static_cast<int>(clickedTarget->GetPos() + (8 * i) - i))) break;
		movableArea.push_back(static_cast<int>(clickedTarget->GetPos() + (8 * i) - i));
		if (isThereEnemy(static_cast<int>(clickedTarget->GetPos() + (8 * i) - i)))
		{
			attackArea.push_back(static_cast<int>(clickedTarget->GetPos() + (8 * i) - i));
			break;
		}
		if (clickedTarget->IsInEdge(static_cast<int>(clickedTarget->GetPos() + (8 * i) - i))) break;
	}

	///South East
	for (std::size_t i = 1; southEast != false; ++i)
	{
		if (isThereAlly(static_cast<int>(clickedTarget->GetPos() + (8 * i) + i))) break;
		movableArea.push_back(static_cast<int>(clickedTarget->GetPos() + (8 * i) + i));
		if (isThereEnemy(static_cast<int>(clickedTarget->GetPos() + (8 * i) + i)))
		{
			attackArea.push_back(static_cast<int>(clickedTarget->GetPos() + (8 * i) + i));
			break;
		}
		if (clickedTarget->IsInEdge(static_cast<int>(clickedTarget->GetPos() + (8 * i) + i))) break;
	}
}

void ChessObjectManager::AdjustMovableAreaRook()
{
	std::size_t right{}, left{}, forward{}, backward{};

	///Calculate count of Movable Area
	left = ((clickedTarget->GetPos() % 8) + 1) - 1;
	right = 8 - left - 1;
	forward = ((clickedTarget->GetPos() / 8) + 1) - 1;
	backward = 8 - forward - 1;

	for (std::size_t i = 1; i <= left; i++)											///Left Area
	{
		if (isThereAlly(static_cast<int>(clickedTarget->GetPos() - i)))				///if there is Ally at end of this line
			break;
		if (isThereEnemy(static_cast<int>(clickedTarget->GetPos() - i)))			///if there is Ally at end of this line
		{
			movableArea.push_back(static_cast<int>(clickedTarget->GetPos() - i));
			attackArea.push_back(static_cast<int>(clickedTarget->GetPos() - i));
			break;
		}
		movableArea.push_back(static_cast<int>(clickedTarget->GetPos() - i));
	}

	for (std::size_t i = 1; i <= right; i++)										///Right Area
	{
		if (isThereAlly(static_cast<int>(clickedTarget->GetPos() + i)))				///if there is Ally at end of this line
			break;
		if (isThereEnemy(static_cast<int>(clickedTarget->GetPos() + i)))			///if there is Ally at end of this line
		{
			movableArea.push_back(static_cast<int>(clickedTarget->GetPos() + i));
			attackArea.push_back(static_cast<int>(clickedTarget->GetPos() + i));
			break;
		}
		movableArea.push_back(static_cast<int>(clickedTarget->GetPos() + i));
	}

	for (std::size_t i = 1; i <= forward; i++)										///foward Area
	{
		if (isThereAlly(static_cast<int>(clickedTarget->GetPos() - i * 8)))			///if there is Ally at end of this line
			break;
		if (isThereEnemy(static_cast<int>(clickedTarget->GetPos() - i * 8)))		///if there is Ally at end of this line
		{
			movableArea.push_back(static_cast<int>(clickedTarget->GetPos() - i * 8));
			attackArea.push_back(static_cast<int>(clickedTarget->GetPos() - i * 8));
			break;
		}
		movableArea.push_back(static_cast<int>(clickedTarget->GetPos() - i * 8));
	}

	for (std::size_t i = 1; i <= backward; i++)										///Backward Area
	{
		if (isThereAlly(static_cast<int>(clickedTarget->GetPos() + i * 8)))			///if there is Ally at end of this line
			break;
		if (isThereEnemy(static_cast<int>(clickedTarget->GetPos() + i * 8)))		///if there is Ally at end of this line
		{
			movableArea.push_back(static_cast<int>(clickedTarget->GetPos() + i * 8));
			attackArea.push_back(static_cast<int>(clickedTarget->GetPos() + i * 8));
			break;
		}
		movableArea.push_back(static_cast<int>(clickedTarget->GetPos() + i * 8));
	}
}

void ChessObjectManager::ResetMovableAreas()
{
	movableArea.clear();
	attackArea.clear();
	clickedTarget = nullptr;
}

ChessObjectManager::ChessObjectManager()
	:clickedTarget(nullptr) {}

ChessObjectManager::~ChessObjectManager()
{
	std::size_t objectsSize = chessObjects.size();

	for (std::size_t i = 0; i < objectsSize; i++)
	{
		delete chessObjects.back();
		chessObjects.pop_back();
	}
}

void ChessObjectManager::InitChessObject()
{
	std::size_t chessPiecesSize = chessObjects.size();

	for (std::size_t i = 0; i < chessPiecesSize; i++)
		delete chessObjects[i];

	chessObjects.clear();

	///Enemy Pieces
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Rook(0, TEAM_TYPE::ENEMY, PIECE_TYPE::R_ROOK)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Knight(1, TEAM_TYPE::ENEMY, PIECE_TYPE::R_KNIGHT)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Bishop(2, TEAM_TYPE::ENEMY, PIECE_TYPE::R_BISHOP)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Queen(3, TEAM_TYPE::ENEMY, PIECE_TYPE::QUEEN)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new King(4, TEAM_TYPE::ENEMY, PIECE_TYPE::KING)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Bishop(5, TEAM_TYPE::ENEMY, PIECE_TYPE::L_BISHOP)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Knight(6, TEAM_TYPE::ENEMY, PIECE_TYPE::L_KNIGHT)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Rook(7, TEAM_TYPE::ENEMY, PIECE_TYPE::L_ROOK)));
	for (int i = 8; i < 16; i++)
		chessObjects.push_back(dynamic_cast<ChessObject*>(new Pawn(i, TEAM_TYPE::ENEMY, static_cast<PIECE_TYPE>(static_cast<int>(PIECE_TYPE::PAWN8) + 8 - i))));


	///Ally Pieces
	for (int i = 48; i < 56; i++)
		chessObjects.push_back(dynamic_cast<ChessObject*>(new Pawn(i, TEAM_TYPE::ALLY, static_cast<PIECE_TYPE>(static_cast<int>(PIECE_TYPE::PAWN1)  + i-48))));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Rook(56, TEAM_TYPE::ALLY, PIECE_TYPE::L_ROOK)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Knight(57, TEAM_TYPE::ALLY, PIECE_TYPE::L_KNIGHT)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Bishop(58, TEAM_TYPE::ALLY, PIECE_TYPE::L_BISHOP)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new King(59, TEAM_TYPE::ALLY, PIECE_TYPE::KING)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Queen(60, TEAM_TYPE::ALLY, PIECE_TYPE::QUEEN)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Bishop(61, TEAM_TYPE::ALLY, PIECE_TYPE::R_BISHOP)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Knight(62, TEAM_TYPE::ALLY, PIECE_TYPE::R_KNIGHT)));
	chessObjects.push_back(dynamic_cast<ChessObject*>(new Rook(63, TEAM_TYPE::ALLY, PIECE_TYPE::R_ROOK)));
}

void ChessObjectManager::DrawBottom(CDC& memDC) const
{
	CBrush blackBrush(RGB(255, 255, 255));
	CBrush whiteBrush(RGB(0, 0, 0));
	for (std::size_t i = 0; i < 8; i++)
	{
		int isEvenLine = (i % 2 == 0) ? 1 : 0;

		for (std::size_t j = (i * 8); j < (i * 8) + 8; j++)
		{
			if (j % 2 == isEvenLine)
				memDC.SelectObject(&blackBrush);
			else
				memDC.SelectObject(&whiteBrush);

			std::pair<int, int> pair = GetPosFromGrid(static_cast<int>(j));
			memDC.Rectangle(pair.first - 40, pair.second - 40, pair.first + 40, pair.second + 40);
		}
	}
}

void ChessObjectManager::DrawMovableArea(CDC& memDC) const
{
	std::size_t movableAreaSize{ movableArea.size() };
	CBrush movableAreaColor(RGB(135, 206, 235));
	CBrush* oldBrush = memDC.SelectObject(&movableAreaColor);

	for (std::size_t i = 0; i < movableAreaSize; i++)
	{
		std::pair<int, int> pair = GetPosFromGrid(movableArea[i]);
		memDC.Rectangle(pair.first - 40, pair.second - 40, pair.first + 40, pair.second + 40);
	}

	memDC.SelectObject(oldBrush);
}

void ChessObjectManager::DrawAttackArea(CDC& memDC) const
{
	std::size_t attackAreaSize{ attackArea.size() };
	CBrush movableAreaColor(RGB(255, 0, 0));
	CBrush* oldBrush = memDC.SelectObject(&movableAreaColor);

	for (std::size_t i = 0; i < attackAreaSize; i++)
	{
		std::pair<int, int> pair = GetPosFromGrid(attackArea[i]);
		memDC.Rectangle(pair.first - 40, pair.second - 40, pair.first + 40, pair.second + 40);
	}

	memDC.SelectObject(oldBrush);
}

void ChessObjectManager::DrawPieces(CDC& memDC) const
{
	std::size_t chessPeiceCount = chessObjects.size();

	for (std::size_t i = 0; i < chessPeiceCount; i++)
		chessObjects[i]->Draw(memDC, 80);
}

bool ChessObjectManager::isThereAlly(int gridNum) const
{
	std::size_t chessPeiceCount = chessObjects.size();

	for (std::size_t i = 0; i < chessPeiceCount; i++)
	{
		if (chessObjects[i]->GetPos() == gridNum)
			return chessObjects[i]->GetTeamType() == TEAM_TYPE::ALLY ? true : false;
	}

	return false;
}

bool ChessObjectManager::isThereEnemy(int gridNum) const
{
	std::size_t chessPeiceCount = chessObjects.size();

	for (std::size_t i = 0; i < chessPeiceCount; i++)
	{
		if (chessObjects[i]->GetPos() == gridNum)
			return chessObjects[i]->GetTeamType() == TEAM_TYPE::ENEMY ? true : false;
	}

	return false;
}



bool ChessObjectManager::GetClickedTarget(CPoint point)
{
	///Detect isThere Peice?
	int clickedGrid{ GetGridFromPos(std::make_pair(point.x,point.y)) };
	///Find Clicked Object or None
	std::size_t objectsCount{ chessObjects.size() };
	for (std::size_t i = 0; i < objectsCount; i++)
	{
		clickedTarget = (chessObjects[i]->GetPos() == clickedGrid) ? chessObjects[i] : nullptr;
		if (clickedTarget != nullptr)
			break;
	}
	
	///If there is nothing clicked or Clicked Objectis Enemy's Object
	///judge left frist and judge right
	if (clickedTarget == nullptr || clickedTarget->GetTeamType() == TEAM_TYPE::ENEMY)
	{
		clickedTarget = nullptr;
		return false;
	}
	
	return true;
}

bool ChessObjectManager::MovePeice(CPoint point)
{
	///clickedTarget is null
	if (clickedTarget == nullptr) return false;
	///Re-Clicked clickedTarget
	int clickedGridNum{ GetGridFromPos(std::make_pair(point.x, point.y)) };
	if (clickedTarget->GetPos() == clickedGridNum)
	{
		ResetMovableAreas();
		return true;
	}

	bool move{ false };
	std::size_t movableAreaCount{ movableArea.size() };

	for (std::size_t i = 0; i < movableAreaCount; i++)
	{
		move = (movableArea[i] == clickedGridNum) ? true : false;
		if (move) break;
	}

	if (move)
	{
		if (isThereEnemy(clickedGridNum))
			deleteObjectFromGrid(TEAM_TYPE::ENEMY, clickedGridNum);
		clickedTarget->SetPos(clickedGridNum);
	}
	else
	{
		AfxMessageBox(_T("올바르지 않은 이동입니다."));
		ResetMovableAreas();
		return false;
	}

	clickedTarget->SetNotFirstMove();		///For Pawn, Not Moving Twice step
	ResetMovableAreas();
	return true;
}

void ChessObjectManager::SetMovableArea()
{
	clickedTarget->SetMovableArea(movableArea);

	switch (clickedTarget->GetPieceType())
	{
	case PIECE_TYPE::PAWN1:
	case PIECE_TYPE::PAWN2:
	case PIECE_TYPE::PAWN3:
	case PIECE_TYPE::PAWN4:
	case PIECE_TYPE::PAWN5:
	case PIECE_TYPE::PAWN6:
	case PIECE_TYPE::PAWN7:
	case PIECE_TYPE::PAWN8:
		AdjustMovableAreaPawn();
		break;
	case PIECE_TYPE::KING:
	case PIECE_TYPE::L_KNIGHT:
	case PIECE_TYPE::R_KNIGHT:
		AdjustMovableAreaNormal();
		break;
	case PIECE_TYPE::L_BISHOP:
	case PIECE_TYPE::R_BISHOP:
		AdjustMovableAreaBishop();
		break;
	case PIECE_TYPE::L_ROOK:
	case PIECE_TYPE::R_ROOK:
		AdjustMovableAreaRook();
		break;
	case PIECE_TYPE::QUEEN:
		AdjustMovableAreaRook();
		AdjustMovableAreaBishop();
		break;
	default:
		break;
	}
}

bool ChessObjectManager::MovePeice(TEAM_TYPE teamType, PIECE_TYPE pieceType, int MovePos)
{
	std::size_t objectsSize = chessObjects.size();

	for (std::size_t i = 0; i < objectsSize; i++)
	{
		if (chessObjects[i]->GetPieceType() == pieceType&&chessObjects[i]->GetTeamType() == teamType)
		{
			chessObjects[i]->SetPos(MovePos);
			return true;
		}
	}

	return false;
}

ChessObject* ChessObjectManager::GetClickedObject()
{
	return clickedTarget;
}