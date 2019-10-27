#pragma once
#include <mutex>
using std::mutex;

#define ROW 12
#define COL 10
typedef BOOL BoardState[ROW][COL];
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
	double m_rank;
	BOOL m_canPlace;
	Position *m_supPos;
	static const BOOL (*TetrisShape)[4][16];
	static BoardState * m_pBoard;
private:
	ConnectRegion m_CRGridCount;
	int m_CRCount[5];

public:
	Position* GetSupremePos();
	Position* PlaceToSupremePos();
	double GetSupremeRank();
	void BindBoard(BoardState * pBoard);
	void PlaceTetris(int x, int y, int r);
	void PlaceTetris(Position * pos);
	void RemoveTetris(int x, int y, int r);
	static void PlaceTetris(int type, Position * pos);
	static void ReverseBlock(int x, int y);
	static void SetBlock(int x, int y);
	static void RemoveBlock(int x, int y);
	static void ClearBoard();
private:
	BOOL FindSupremePos();
	BOOL CanPlaceTetris(int x, int y, int r);
	BOOL CanPlaceBlock(int x, int y);
	double Rank(int x, int y, int r);
	int CalcLandHeight(int y, int r);
	int CalcEroded();
	int CalcHoles();
	int CalcRowTrans();
	int CalcColTrans();
	void CalcCR();
};


