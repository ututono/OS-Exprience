#include "pch.h"
#include <iostream>

//#define N 20		//分区数
//#define MINSIZE 8		//最小可分割块
//
//#define getpch(type) (type*)malloc(sizeof(type)) 
//
//struct Partitions
//{
//	int num;
//	int size;
//	int firstad;//起始地址
//	bool statue;	//分配状态
//	struct Partitions *before;
//	struct Partitions *after;
//}*Indicate,*Node;
//
//typedef struct  Partitions PAT;
//
//void InitTable()
//{
//
//	int i, data=0;
//	Indicate =(Partitions*)malloc(sizeof(Partitions));
//	Partitions *pTail = NULL,*pnew=NULL;
//	Indicate->num =0;
//	Indicate->size = 2;
//	Indicate->statue = 0;
//	Indicate->firstad = rand() % 10;
//	Indicate->after = NULL;
//	Indicate->before = NULL;
//	pTail = Indicate;
//
//	for (int i = 0; i < N; i++)
//	{
//		pnew = (Partitions*)malloc(sizeof(Partitions));
//		pnew->num = i;
//		pnew->size = i * 4;
//		pnew->statue = 0;
//		pnew->firstad = i * 8 + rand() % 10;
//		pnew->before = NULL;
//		pnew->after= pTail;
//
//		pTail = pnew;
//	}
//
//
//	//for (int i = 0; i < N; i++)
//	//{
//	//	if (i == 1)
//	//	{
//	//		Indicate->before = NULL;
//	//	}
//	//	else Indicate->before = NULL;
//	//	Indicate->num = i;
//	//	Indicate->size = i * 4;
//	//	Indicate->statue = 0;
//	//	Indicate->firstad = i * 8 + rand() % 10;
//	//	Indicate->num = i;
//	//	Indicate->size = i * 4;
//	//	Indicate->statue = 0;
//	//	Indicate->firstad = i * 8 + rand() % 10;
//
//	//	temp->num = i;
//	//	temp->size = i * 4;
//	//	temp->statue = 0;
//	//	temp->firstad = i * 8 + rand() % 10;
//	//	temp->num = i;
//	//	temp->size = i * 4;
//	//	temp->statue = 0;
//	//	temp->firstad = i * 8 + rand() % 10;
//	//	temp->before = Indicate;
//	//}
//}
//
//int main()
//{
//	InitTable();
//	Partitions *temp= Indicate;
//	system("pause");
//	return 0;
//}

