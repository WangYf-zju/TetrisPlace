#include "stdafx.h"
#include "Parameter.h"


int StoreRowBoundary[STORE_ROW_BOUNDARY_COUNT] = {
	DEFAULT_STORE_ROW_START,
	DEFAULT_STORE_ROW_START + 1 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 2 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 3 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 4 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 5 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 6 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 7 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 8 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 9 * DEFAULT_STORE_ROW_DISTANCE,
	DEFAULT_STORE_ROW_START + 10 * DEFAULT_STORE_ROW_DISTANCE
};
int StoreColBoundary[STORE_COL_BOUNDARY_COUNT] = {
	DEFAULT_STORE_COL_START,
	DEFAULT_STORE_COL_START + 1 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 2 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 3 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 4 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 5 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 6 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 7 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 8 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 9 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 10 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 11 * DEFAULT_STORE_COL_DISTANCE,
	DEFAULT_STORE_COL_START + 12 * DEFAULT_STORE_COL_DISTANCE
};
int * CParameter::pStoreRowBoundary = StoreRowBoundary;
int * CParameter::pStoreColBoundary = StoreColBoundary;

int CParameter::startX = DEFAULT_ARM_STARTX;
int CParameter::startY = DEFAULT_ARM_STARTY;
int CParameter::startZ = DEFAULT_ARM_STARTZ;

int CParameter::offsetX1 = DEFAULT_OFFSETX1;
int CParameter::offsetY1 = DEFAULT_OFFSETY1;
int CParameter::offsetX2 = DEFAULT_OFFSETX2;
int CParameter::offsetY2 = DEFAULT_OFFSETY2;
CParameter::CParameter()
{
}


CParameter::~CParameter()
{
}
