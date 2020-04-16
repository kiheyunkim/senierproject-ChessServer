#ifndef _PAWN_H_
#define _PAWN_H_

class Pawn :public ChessObject
{
private:

public:
	Pawn(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType);
	virtual ~Pawn();
	virtual void SetMovableArea(std::vector<int>& movableArea);
};

#endif // !_PAWN_H_