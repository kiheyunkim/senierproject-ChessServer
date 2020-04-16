#ifndef _CHESSOBJECTMANAGER_H_
#define _CHESSOBJECTMANAGER_H_

class ChessObjectManager
{
private:
	std::vector<ChessObject*> chessObjects;
	std::vector<int> movableArea;
	std::vector<int> attackArea;
	ChessObject* clickedTarget;

private:
	std::pair<int, int> GetPosFromGrid(int gridNum) const;
	int GetGridFromPos(std::pair<int, int> pair) const;
	void AdjustMovableAreaPawn();
	void AdjustMovableAreaNormal();
	void AdjustMovableAreaBishop();
	void AdjustMovableAreaRook();
	void ResetMovableAreas();

public:
	ChessObjectManager();
	~ChessObjectManager();
	bool deleteObjectFromGrid(TEAM_TYPE teamType,int gridNum);
	void InitChessObject();
	void DrawBottom(CDC& memDC)			const;
	void DrawMovableArea(CDC& memDC)	const;
	void DrawAttackArea(CDC& memDC)		const;
	void DrawPieces(CDC& memDC)			const;

public:
	bool isThereAlly(int gridNum)		const;
	bool isThereEnemy(int gridNum)		const;
	bool GetClickedTarget(CPoint point);
	bool MovePeice(CPoint point);
	void SetMovableArea();
	ChessObject* GetClickedObject();

public:
	bool MovePeice(TEAM_TYPE teamType, PIECE_TYPE pieceType, int MovePos);
};

#endif