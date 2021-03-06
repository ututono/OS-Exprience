#include "pch.h"
#include <iostream>

#define ERR -1
#define GROUPNUM 256			//有246组
#define PERGROUP_CAPACITY 100			//每组100块物理块
#define MAX_UNIT 4							//每个物理块4KB
#define MAX_FILE 10					//文件目录最多的文件数
#define MAX_FILE_CAPACITY 3840		//单个文件最大容量15MB
#define MAX_FILE_NAME 20				//文件名最大长度

#pragma warning(disable:4996)

typedef int ElemType;
typedef bool Status;
using namespace std;

bool mutex = 1;			//互斥变量

/*物理块数据结构*/
struct FreeStack
{
	int N =0;							//本组空闲盘块数
	bool apply = 0;				//是否被占用，0为空闲
	ElemType free[PERGROUP_CAPACITY];
	ElemType FileIndex[MAX_FILE_CAPACITY];				//索引块
	int FileBlockNum = 0;					//文件所占物理块数
}SuperBlock,Storage[GROUPNUM*PERGROUP_CAPACITY];

/*文件目录的数据结构*/
struct DIR
{
	ElemType FirstAddr[MAX_FILE];					//起始物理块
	char Name[MAX_FILE][MAX_FILE_NAME];								//文件名
	int num=0;																	//	文件项数
}dir;

int used[GROUPNUM*PERGROUP_CAPACITY];		//	已使用的内存编号
int Cout = 0;		//已使用的物理快个数

/*初始化缓冲数组*/
void InitUsed()
{
	memset(used, -1, sizeof(int)*GROUPNUM*PERGROUP_CAPACITY);			//将缓冲数组清空
	Cout = 0;
}

/*初始化内存，将所有块都链接*/
Status InitBuffer()
{
	int i = (PERGROUP_CAPACITY*GROUPNUM);
	while (i >0)
	{						//初始化物理块
		i--;
		if (i%PERGROUP_CAPACITY == 0)
		{
			Storage[i].N = PERGROUP_CAPACITY;
			int j = 1,m=PERGROUP_CAPACITY-1;
			while (j<=PERGROUP_CAPACITY)
			{
				Storage[i].free[m] = i-j;
				j++;
				m--;
			}
		}
		else
		{
				memset(Storage[i].free, -1, sizeof(int)*PERGROUP_CAPACITY);
		}
	}
	InitUsed();
	return true;
}

/*初始化超级块*/
Status InitSuperBlock()
{
	SuperBlock.N = PERGROUP_CAPACITY;
	int SUMBlock = PERGROUP_CAPACITY * GROUPNUM;
	int temp = 0;
	for (int i = SUMBlock; i >0; i--)
	{					//找到末尾一个物理块目录
		if (Storage[i].N > 0)
		{
			temp = i;
			break;
		}
	}
	for (int i =PERGROUP_CAPACITY-1,j= SUMBlock-1; i >=0; i--,j--)
	{
		SuperBlock.free[i] = j;
	}
	return true;
}

/*将内存分配给作业,输入：物理块编号*/
Status DistriToJob(int freeBlock)
{
	used[Cout] = freeBlock;
	Cout++;
	return true;
}

/*
分配函数：将空闲盘块分配出去
输入：需要分配的物理块数
*/
int Distri(int nBlock)
{
	int m = SuperBlock.N - 1;				//m--->栈顶
	int res = nBlock - SuperBlock.N;

	if (nBlock >= SuperBlock.N)
	{
		while (m>0)
		{
			if (Storage[SuperBlock.free[m]].apply == 0)
			{
				Storage[SuperBlock.free[m]].apply = 1;
				DistriToJob(SuperBlock.free[m]);
				SuperBlock.free[m] = -1;
				m--;
			}
			else
			{
				printf("ERROR:can't find free block");
				return ERR;
			}
		}
		if (Storage[SuperBlock.free[0]].apply == 1||SuperBlock.free[0]==0)			//若空闲栈的0号元素所指的物理块被占用，则出错
		{
			printf("ERROR:can't find free block");
			return ERR;
		}
		else
		{			//将原栈底分配出去，将其对应的栈复制进空闲栈中
			if (SuperBlock.free[0] == 0) return -1;				//如果到最后一个物理块，返回-1
			Storage[SuperBlock.free[0]].apply = 1;
			DistriToJob(SuperBlock.free[0]);
			Storage[SuperBlock.free[0]].N =0;
			int num0 = SuperBlock.free[0];
			for (int j = 0; j < PERGROUP_CAPACITY; j++)
			{
				SuperBlock.free[j] = Storage[num0].free[j];
			}
			SuperBlock.N = PERGROUP_CAPACITY;
			if(-1==Distri(res)) return ERR;
			return true;
		}
	}
	else				//如果所需物理块小于N
	{
		for (int j = 0; j < nBlock; j++)
		{
			Storage[SuperBlock.free[m]].apply = 1;
			DistriToJob(SuperBlock.free[m]);
			SuperBlock.free[m] = -1;
			m--;
		}
		SuperBlock.N -= nBlock;
		return true;
	}
}

/*回收物理块号
输入：回收的物理快号
输出：
*/
Status Recover(int freeBlock)
{
	if ((SuperBlock.N + 1) > PERGROUP_CAPACITY)			//空闲栈中物理盘块数大于每组大小
	{				
		for (int i = 0; i < PERGROUP_CAPACITY; i++)
		{
			Storage[freeBlock].free[i] = SuperBlock.free[i];
			SuperBlock.free[i] = -1;
			Storage[freeBlock].apply= 0;
		}
		SuperBlock.free[0] = freeBlock;
		SuperBlock.N = PERGROUP_CAPACITY;
	}
	else
	{
		SuperBlock.N++;
		SuperBlock.free[SuperBlock.N - 1] = freeBlock;
		Storage[freeBlock].apply = 0;
	}
	return true;
}



/*
文件分配
输入：文件名，文件大小
修改：文件目录、物理块空闲栈
*/
void FileDistr(char *name,int size)
{
	int Blocknum = 0;
	int indexaddr = 0;
	if (size%MAX_UNIT != 0)			//文件所需物理块数
		Blocknum = size / MAX_UNIT + 1;
	else Blocknum = size / MAX_UNIT;
	if (Blocknum> GROUPNUM*PERGROUP_CAPACITY)
	{
		printf("\n--------FBI WRANING--------\n");
		printf("  ERROR:超过最大存储空间\n\n");
		return;
	}
	InitUsed();
	if (ERR == Distri(1))
	{
		printf("分配失败（QAQ）\n");
		return;											//为索引块分配空间
	}
	indexaddr = used[0];
	Storage[indexaddr].FileBlockNum = Blocknum;
	InitUsed();
	if (ERR == Distri(Blocknum))
	{
		int i = 0;
		while (Storage[indexaddr].FileIndex[i] >= 0)
		{
			Recover(Storage[indexaddr].FileIndex[i]);
			i++;
		}
	}
	for (int i = 0; i < Blocknum; i++)
	{
		Storage[indexaddr].FileIndex[i] = used[i];
	}
	strcpy_s(dir.Name[dir.num],name);			//更新文件目录
	dir.FirstAddr[dir.num] = indexaddr;
	dir.num++;
	printf("*****分配成功！！！******\n");
}

/*
文件回收
输入：文件名
修改：文件目录、物理块空闲栈
*/
void FileRecover(char *name) 
{
	int tag = -1;			//文件存在的标志
	for (int i = 0; i < dir.num; i++)
	{
		if (strcmp(name, dir.Name[i]) == 0)
		{
			tag = i;
			break;
		}
	}
	if (tag == -1)
	{
		printf("该文件不存在，请检查输入\n");
		return;
	}
	else
	{
		for (int i = 0; i < Storage[dir.FirstAddr[tag]].FileBlockNum; i++)
		{			//回收索引块中记录的所有物理块
			Recover(Storage[dir.FirstAddr[tag]].FileIndex[i]);
		}
		Recover(dir.FirstAddr[tag]);
		while (tag<= dir.num)			//修改文件目录
		{
			dir.FirstAddr[tag] = dir.FirstAddr[tag + 1];
			strcpy_s(dir.Name[tag],dir.Name[tag + 1]);
			tag++;
		}
		dir.num--;
		printf("回收成功 ! \n");
	}
}

void Display()
{
	printf("***********SuperBlock*************\n");
	printf("N=%d\n", SuperBlock.N);
	for (int i = 0; i < PERGROUP_CAPACITY; i++)
	{
		printf("%d\n", SuperBlock.free[i]);
	}
	printf("***********SuperBlock*************\n");
	printf("************Used************\n");
	for (int i = 0; i < PERGROUP_CAPACITY*2; i++)
	{
		printf("%d\n", used[i]);
	}
	printf("************Used************\n");
}

void ElemDisplay(int num)
{
	printf("************ELemDisplay************\n");
	printf("\tapply\tFileBlock\tFileIndex\tfree\tN\n");
	printf("\t%d\t%d\t%d\t%d\t%d\n",Storage[num].apply,Storage[num].FileBlockNum,Storage[num].FileIndex[0],Storage[num].free[0],Storage[num].N);
	printf("**********___END___**********\n");
}

void DirDisplay()
{
	printf("****************\n");
	for (int i = 0; i < dir.num; i++)
	{
		printf("Name\tIndexaddr\tBlocknum\n");
		printf("%s\t%d\t\t%d\n", dir.Name[i], dir.FirstAddr[i], Storage[dir.FirstAddr[i]].FileBlockNum);
	}
	printf("****************\n");
}

void StorageDisplay()
{
	int i = 0;
	printf("请输入起始物理块号:");
	scanf("%d", &i);
	for (; i < PERGROUP_CAPACITY*GROUPNUM; i++)
	{
		if (i%PERGROUP_CAPACITY == 0) printf("\n\n");
		printf("%d:  %d\t", i, Storage[i].apply);
	}
}

/*同名检查函数*/
bool checksame(char *name)
{
	for (int i = 0; i < dir.num; i++)
	{
		if (!strcmp(name, dir.Name[i]))
			return false;
	}
	return true;
}

void Help()
{
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t\t-----------------------------------\n");
	printf("\t\t\t\t\t\t|         1. 分配文件             |\n");
	printf("\t\t\t\t\t\t|         2. 释放文件             |\n");
	printf("\t\t\t\t\t\t|         3. 查看文件目录表       |\n");
	printf("\t\t\t\t\t\t|         0. 退出exit             |\n");
	printf("\t\t\t\t\t\t-----------------------------------\n");
	printf("\t\t\t\t\t\t              请输入:_");
}

int main()
{
	InitBuffer();
	InitSuperBlock();
	int choose = 1;
	while (choose)
	{
		system("cls");
		Help();
		scanf_s("%d", &choose);
		switch (choose)
		{
		case 1:
		{
			char name[MAX_FILE_NAME];
			int size;
			strcpy_s(name, "A");
			printf("请输入文件名：");
			scanf("%s", name);
			if (false == checksame(name))
			{
				printf("ERROR:不允许有重名文件\n");
				break;
			}
			size = 64;
			printf("\n请输入文件大小（单位KB）：");
			scanf("%d", &size);
			FileDistr(name, size);
		}
		break;
		case 2:
		{
			char name[MAX_FILE_NAME];
			DirDisplay();
			printf("请输入要释放的文件名：");
			scanf("%s", name);
			FileRecover(name);
		}
		break;
		case 3:
		{
			DirDisplay();
		}
		break;
		case 4:
		{
			int num = 0;
			printf("要查看的物理块号：");
			scanf("%d", &num);
			ElemDisplay(num);
		}
		break;
		case 5:
		{
			StorageDisplay();
		}
		break;
		default:
			break;
		}
		system("pause");
	}
	return 0;
}

