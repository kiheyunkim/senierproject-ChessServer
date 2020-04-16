#ifndef _QUEEN_H_
#define _QUEEN_H_

class Queen :public ChessObject
{
public:
	Queen(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType);
	~Queen();
	virtual void SetMovableArea(std::vector<int>& movableArea);
};

#endif // !_QUEEN_H_
