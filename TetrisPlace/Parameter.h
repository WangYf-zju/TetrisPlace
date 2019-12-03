#pragma once
class CParameter
{
public:
	CParameter();
	~CParameter();

	static int * pStoreRowBoundary;
	static int * pStoreColBoundary;

	static int startX;
	static int startY;
	static int startZ;

	static int offsetX1;
	static int offsetY1;
	static int offsetX2;
	static int offsetY2;
};

