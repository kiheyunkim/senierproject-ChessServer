#ifndef _BISHOP_H_
#define _BISHOP_H_

class Bishop :public ChessObject
{
public:
	Bishop(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType);
	~Bishop();
	virtual void SetMovableArea(std::vector<int>& movableArea);
};

#endif // !_BISHOP_H_
