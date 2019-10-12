#pragma once
#define RAW 12
#define COL 10
typedef BOOL BoardState[RAW][COL];
typedef int ConnectRegion[50];

typedef struct {
	int x;
	int y;
	int r;
}Position;

class TetrisAI
{
public:
	TetrisAI();
	TetrisAI(int type);
	~TetrisAI();
public:
	int m_type;
	Position *m_supPos;
	static const BOOL (*TetrisShape)[4][16];
	static BoardState * m_pBoard;
private:
	ConnectRegion m_CRGridCount;
	int m_CRCount[5];

public:
	Position* GetSupremePos();
	Position* PlaceToSupremePos();
	void BindBoard(BoardState * pBoard);
	void PlaceTetris(int x, int y, int r);
	void PlaceTetris(Position * pos);
	void RemoveTetris(int x, int y, int r);
private:
	BOOL CanPlaceTetris(int x, int y, int r);
	BOOL CanPlaceBlock(int x, int y);
	double Rank(int x, int y, int r);
	int CalcLandHeight(int y, int r);
	int CalcEroded();
	int CalcHoles();
	int CalcRawTrans();
	int CalcColTrans();
	void CalcCR();
};


