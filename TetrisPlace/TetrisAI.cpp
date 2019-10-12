#include "stdafx.h"
#include "TetrisAI.h"
#include <time.h>
#include <stdlib.h>

const BOOL TetrisShapeArray[7][4][16] =
{
{{1,0,0,0,1,1,1},{0,1,1,0,0,1,0,0,0,1},{0,0,0,0,1,1,1,0,0,0,1},{0,1,0,0,0,1,0,0,1,1}},
{{0,0,1,0,1,1,1},{0,1,0,0,0,1,0,0,0,1,1},{0,0,0,0,1,1,1,0,1},{1,1,0,0,0,1,0,0,0,1}},
{{0,1,1,0,1,1},{1,0,0,0,1,1,0,0,0,1},{0,1,1,0,1,1},{1,0,0,0,1,1,0,0,0,1}},
{{1,1,0,0,0,1,1},{0,0,1,0,0,1,1,0,0,1},{1,1,0,0,0,1,1},{0,0,1,0,0,1,1,0,0,1}},
{{1,1,0,0,1,1},{1,1,0,0,1,1},{1,1,0,0,1,1},{1,1,0,0,1,1}},
{{0,0,0,0,1,1,1,1},{0,1,0,0,0,1,0,0,0,1,0,0,0,1},
	{0,0,0,0,1,1,1,1},{0,1,0,0,0,1,0,0,0,1,0,0,0,1}},
{{0,1,0,0,1,1,1},{0,1,0,0,0,1,1,0,0,1},{0,0,0,0,1,1,1,0,0,1},{0,1,0,0,1,1,0,0,0,1}}
};

BoardState board = { 0 };

const BOOL(*TetrisAI::TetrisShape)[4][16] = TetrisShapeArray;
BoardState* TetrisAI::m_pBoard = &board;

TetrisAI::TetrisAI()
{
	m_type = rand() % 7;
	m_supPos = new Position;
	m_supPos->x = 0;
	m_supPos->y = 0;
	m_supPos->r = 0;
}

TetrisAI::TetrisAI(int type)
{
	if (type < 0 || type>6)
	{
		m_type = rand() % 7;
	}
	else
	{
		m_type = type;
	}
	m_supPos = new Position;
	m_supPos->x = 0;
	m_supPos->y = 0;
	m_supPos->r = 0;
}


TetrisAI::~TetrisAI()
{
	delete m_supPos;
}


Position* TetrisAI::GetSupremePos()
{	
	BOOL canPlace = FALSE;
	double max = -1e10;
	for (int x = -1; x < COL; x++)
	{
		for (int y = -1; y < RAW; y++)
		{
			for (int r = 0; r < 4; r++)
			{
				if (CanPlaceTetris(x, y, r))
				{
					canPlace = TRUE;
					double rank = Rank(x, y, r);
					if (rank > max)
					{
						max = rank;
						m_supPos->x = x;
						m_supPos->y = y;
						m_supPos->r = r;
					}
				}
			}
		}
	}
	if (!canPlace)
		return nullptr;
	return m_supPos;
}

Position* TetrisAI::PlaceToSupremePos()
{
	Position * pos = GetSupremePos();
	if (nullptr == pos)
		return nullptr;
	PlaceTetris(m_supPos);
	return m_supPos;
}


BOOL TetrisAI::CanPlaceTetris(int x, int y, int r)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
 			if (TetrisShape[m_type][r][j * 4 + i] && !CanPlaceBlock(x + i, y + j))
				return FALSE;
		}
	}
	return TRUE;
}


BOOL TetrisAI::CanPlaceBlock(int x, int y)
{
	if (x < 0 || x >= COL || y < 0 || y >= RAW || (*m_pBoard)[y][x])
		return FALSE;
	return TRUE;
}


void TetrisAI::BindBoard(BoardState * pBoard)
{
	m_pBoard = pBoard;
}


double TetrisAI::Rank(int x, int y, int r)
{
	int LH = CalcLandHeight(y, r);
	int oldER = CalcEroded();
	int oldHole = CalcHoles();
	int oldRT = CalcRawTrans();
	int oldCT = CalcColTrans();
	CalcCR();
	int oldCR1 = m_CRCount[0];
	int oldCR2 = m_CRCount[0];
	int oldCR3 = m_CRCount[0];
	PlaceTetris(x, y, r);
	int dER = CalcEroded() - oldER;
	int dHole = CalcHoles() - oldHole;
	int dRT = CalcRawTrans() - oldRT;
	int dCT = CalcColTrans() - oldCT;
	CalcCR();
	int dCR1 = m_CRCount[0] - oldCR1;
	int dCR2 = m_CRCount[1] - oldCR2;
	int dCR3 = m_CRCount[2] - oldCR3;
	RemoveTetris(x, y, r);
	double rank = -5 * LH + 10 * dER - 3 * dRT - 5 * dCT - 5 * dHole
		- 50 * dCR1 - 75 * dCR2 - 100 * dCR3;
	return rank;
}


int TetrisAI::CalcLandHeight(int y, int r)
{
	for (int i = 3; i >= 0; i--)
	{
		for (int j = 0; j < 4; j++)
		{
			if (TetrisShape[m_type][r][i * 4 + j])
			{
				return y + i;
			}
		}
	}
	return y;
}


int TetrisAI::CalcEroded()
{
	BOOL flag;
	int count = 0;
	for (int y = 0; y < RAW; y++)
	{
		flag = TRUE;
		for (int x = 0; x < COL; x++)
		{
			if (!(*m_pBoard)[y][x])
				flag = FALSE;
		}
		if (flag) count++;
	}
	return count;
}


int TetrisAI::CalcHoles()
{
	BOOL flag;
	int count = 0;
	for (int x = 0; x < RAW; x++)
	{
		flag = FALSE;
		for (int y = COL-1; y >= 0; y--)
		{
			if ((*m_pBoard)[y][x]) flag = TRUE;
			if (flag && !(*m_pBoard)[y][x]) count++;
		}
	}
	return 0;
}


void TetrisAI::PlaceTetris(int x, int y, int r)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (TetrisShape[m_type][r][4 * j + i])
			{
				(*m_pBoard)[y + j][x + i] = TRUE;
			}
		}
	}
}

void TetrisAI::PlaceTetris(Position * pos)
{
	PlaceTetris(pos->x, pos->y, pos->r);
}


void TetrisAI::RemoveTetris(int x, int y, int r)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (TetrisShape[m_type][r][4 * j + i])
			{
				(*m_pBoard)[y + j][x + i] = FALSE;
			}
		}
	}
}

int TetrisAI::CalcRawTrans()
{
	int count = 0;
	for (int y = 0; y < RAW; y++)
	{
		if (!(*m_pBoard)[0][y]) count++;
		for (int x = 1; x < COL; x++)
		{			
			if ((*m_pBoard)[y][x] != (*m_pBoard)[y][x - 1])
				count++;
		}
		if (!(*m_pBoard)[y][COL - 1]) count++;
	}
	return count;
}


int TetrisAI::CalcColTrans()
{
	int count = 0;
	for (int x = 0; x < COL; x++)
	{
		if (!(*m_pBoard)[0][x]) count++;
		for (int y = 1; y < RAW; y++)
		{
			if ((*m_pBoard)[y][x] != (*m_pBoard)[y - 1][x])
				count++;
		}
		if (!(*m_pBoard)[RAW - 1][x]) count++;
	}
	return count;
}


void TetrisAI::CalcCR()
{
	for (int i = 0; i < 50; i++)
	{
		m_CRGridCount[i] = 0;
	}
	for (int i = 0; i < 5; i++)
	{
		m_CRCount[i] = 0;
	}
	int order = 0;
	int hOrder = 0;
	int boardFlag[RAW][COL] = { 0 };
	int oldCRStart[5] = { -1,-1,-1,-1,-1 };
	int oldCREnd[5] = { -1,-1,-1,-1,-1 };
	int CRStart[5] = { -1,-1,-1,-1,-1 };
	int CREnd[5] = { -1,-1,-1,-1,-1 };
	for (int y = 0; y < RAW; y++)
	{
		for (int x = 0; x < COL + 1; x++)
		{
			if (CanPlaceBlock(x, y) && !CanPlaceBlock(x - 1, y))
			{
				CRStart[hOrder] = x;
				CREnd[hOrder] = x;
			}
			else if (CanPlaceBlock(x, y) && CanPlaceBlock(x - 1, y))
			{
				CREnd[hOrder] = x;
			}
			else if (!CanPlaceBlock(x, y) && CanPlaceBlock(x - 1, y))
			{
				BOOL flag = FALSE;
				int curOrder = 0;
				for (int i = 0; i < 5; i++)
				{
					if ((CRStart[hOrder] >= oldCRStart[i] && CRStart[hOrder] <= oldCREnd[i])
						|| (CREnd[hOrder] >= oldCRStart[i] && CREnd[hOrder] <= oldCREnd[i])
						|| (oldCRStart[i] >= CRStart[hOrder] && oldCRStart[i] <= CREnd[hOrder])
						|| (oldCREnd[i] >= CRStart[hOrder] && oldCREnd[i] <= CREnd[hOrder]))
					{
						if (!flag)
						{
							curOrder = boardFlag[y - 1][oldCRStart[i]];
							for (int j = CRStart[hOrder]; j <= CREnd[hOrder]; j++)
							{
								boardFlag[y][j] = curOrder;
							}
						}
						else
						{
							int curModOrder = boardFlag[y - 1][oldCRStart[i]];
							for (int yy = 0; yy <= y; yy++)
							{
								for (int xx = 0; xx < COL; xx++)
								{
									if (boardFlag[yy][xx] == curModOrder)
										boardFlag[yy][xx] = curOrder;
								}
							}
						}
						flag = TRUE;						
					}
				}
				if (!flag)
				{
					order++;
					for (int j = CRStart[hOrder]; j <= CREnd[hOrder]; j++)
					{						
						boardFlag[y][j] = order;
					}
				}
				hOrder++;
			}
		}
		for (int i = 0; i < 5; i++)
		{
			oldCRStart[i] = CRStart[i];
			oldCREnd[i] = CREnd[i];
			CRStart[i] = -1;
			CREnd[i] = -1;
		}
		hOrder = 0;
	}
	for (int y = 0; y < RAW; y++)
	{
		for (int x = 0; x < COL; x++)
		{
			if (boardFlag[y][x] > 0)
				m_CRGridCount[boardFlag[y][x] - 1]++;
		}
	}
	for (int i = 0; i < 50; i++)
	{
		switch (m_CRGridCount[i] % 4)
		{
		case 1:
			m_CRCount[0]++;
			break;
		case 2:
			m_CRCount[1]++;
			break;
		case 3:
			m_CRCount[2]++;
			break;
		default:
			break;
		}
	}
}
