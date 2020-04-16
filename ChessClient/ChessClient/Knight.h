#ifndef _KNIGHT_H_
#define _KNIGHT_H_

class Knight :public ChessObject
{
public:
	Knight(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType);
	~Knight();
	virtual void SetMovableArea(std::vector<int>& movableArea);
};

#endif // !_KNIGHT_H_
