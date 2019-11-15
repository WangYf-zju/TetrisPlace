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
};

