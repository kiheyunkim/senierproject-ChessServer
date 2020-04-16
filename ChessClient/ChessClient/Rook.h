#ifndef _ROOK_H_
#define _ROOK_H_

class Rook :public ChessObject
{
private:

public:
	Rook(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType);
	virtual ~Rook();
	virtual void SetMovableArea(std::vector<int>& movableArea);
};

#endif // !_ROOK_H_
