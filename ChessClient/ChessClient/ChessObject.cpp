#include "stdafx.h"
#include "ChessObject.h"

ChessObject::ChessObject(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType)
	:pos(pos), teamType(teamType), pieceType(pieceType), clicked(false), isFirstMove(true) {}

ChessObject::~ChessObject() 
{
	image.Destroy(); 
	objectImage.Destroy();
	selectedObjectImage.Destroy();
}

void ChessObject::setImage(LPCTSTR imageFileName, LPCTSTR selectedImageFileName)
{
	objectImage.Load(imageFileName);
	selectedObjectImage.Load(selectedImageFileName);
	image = objectImage;
}

std::pair<int, int> ChessObject::GetPosFromGrid(int gridNum) const
{
	int x{ 40 + (gridNum % 8 * 80) };
	int y{ 40 + (gridNum / 8 * 80) };

	return std::make_pair(x, y);
}

void ChessObject::SetPieceType(PIECE_TYPE pieceType) { this->pieceType = pieceType; }

void ChessObject::SetPos(int pos) { this->pos = pos; }

TEAM_TYPE ChessObject::GetTeamType() const { return teamType; }

PIECE_TYPE ChessObject::GetPieceType() const { return pieceType; }

int ChessObject::GetPos() const { return pos; }

bool ChessObject::ChangeImage()
{
	if (objectImage.IsNull() || selectedObjectImage.IsNull()) return false;
	image = clicked ? objectImage : selectedObjectImage;
	clicked = clicked ? false : true;

	return true;
}

bool ChessObject::Draw(CDC& dc, int size) const
{
	if (image.IsNull())
		return false;

	int halfSize = size / 2;
	std::pair<int, int> pair = GetPosFromGrid(GetPos());
	if (image.TransparentBlt(dc.m_hDC, pair.first - halfSize, pair.second - halfSize, size, size, RGB(255, 0, 255)))
		return false;

	return true;
}

void ChessObject::SetNotFirstMove()
{
	if (!isFirstMove) return;
	isFirstMove = false;
}

bool ChessObject::IsInEdge(int pos) const { return pos / 8 == 0 || pos / 8 == 7 || (pos % 8) == 0 || (pos % 8) == 7; }