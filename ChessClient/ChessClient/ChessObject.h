#ifndef _CHESSOBJECT_H_
#define _CHESSOBJECT_H_

class ChessObject
{
private:
	bool			clicked;
	CImage			image;
	CImage			objectImage;
	CImage			selectedObjectImage;

private:
	int				pos; 
	TEAM_TYPE		teamType;
	PIECE_TYPE		pieceType;
	
protected:							///Only for Pawn
	bool			isFirstMove;

private:
	std::pair<int, int>		GetPosFromGrid(int gridNum)		const;

protected:
	ChessObject(int pos, TEAM_TYPE teamType, PIECE_TYPE pieceType);
	void					setImage(LPCTSTR imageFileName, LPCTSTR selectedImageFileName);					

public:
	virtual ~ChessObject();
	void					SetPieceType(PIECE_TYPE pieceType);
	void					SetPos(int pos);
	TEAM_TYPE				GetTeamType()					const;
	PIECE_TYPE				GetPieceType()					const;
	int						GetPos()						const;
	bool					ChangeImage();
	bool					Draw(CDC& dc, int size)			const;
	void					SetNotFirstMove();

public:
	bool					IsInEdge(int pos)		const;

public:
	virtual void SetMovableArea(std::vector<int>& movableArea) = 0;
};

#endif // !_CHESSOBJECT_H_