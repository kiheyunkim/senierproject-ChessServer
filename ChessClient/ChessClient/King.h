#ifndef _KING_H_
#define _KING_H_

class King :public ChessObject
{
public:
	King(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType);
	~King();
	virtual void SetMovableArea(std::vector<int>& movableArea);
};
#endif // !_KING_H_
