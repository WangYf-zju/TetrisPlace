#include<stdio.h>
#include<stdlib.h>

struct TNode {
	int Key;
	struct TNode* Left;
	struct TNode* Right;
};
typedef struct TNode* BinTree;

BinTree BuildTree(int N);
BinTree Insert(BinTree T,int K);
int Check(BinTree T1, BinTree T2);
void PrintTree(BinTree T);

int main(void)
{
//	int L, N, i;
	BinTree Tree;
	Tree = BuildTree(5);
	PrintTree(Tree);
	return 0;
}

BinTree BuildTree(int N)
{
	int key;
	BinTree Head = (BinTree)malloc(sizeof(BinTree));

	scanf_s("%d", &key);

	Head->Key = key;
	Head->Left = NULL;
	Head->Right = NULL;

	for (int index = 1; index < N; index++)
	{
		scanf_s("%d", &key);
		Head = Insert(Head, key);
	}
	return Head;
}

BinTree Insert(BinTree T, int K)
{
	if (T!=NULL)
	{
		if (T->Key < K)
			T->Right = Insert(T->Right, K);
		else
			T->Left = Insert(T->Left, K);
	}
	else
	{
		BinTree NewNode = (BinTree)malloc(sizeof(TNode));
		if (NewNode)
		{
			NewNode->Key = K;
			NewNode->Left = NULL;
			NewNode->Right = NULL;
		}

		return NewNode;
	}
	return T;
}

int Check(BinTree T1, BinTree T2)
{
	if ((T1 == NULL && T2 != NULL) || (T1 != NULL && T2 == NULL))
		return 0;
	else if (T1 == NULL && T2 == NULL)
		return 1;
	else
		return Check(T1->Left, T2->Left) & Check(T1->Right, T2->Right);
}

void PrintTree(BinTree T)
{
	if (T)
	{
		PrintTree(T->Left);
		printf("%d ", T->Key);
		PrintTree(T->Right);
	}
}