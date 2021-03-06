// Job Mutl.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#define getpch(type) (type*)malloc(sizeof(type)) 
#define NULL 0 
#define  N 5
#define m_FCFS 1
#define m_SJF 2

int timesize;
int Resourse = 20;

/* 定义进程控制块JCB */
struct jcb {
	char name[10];
	char state;
	int super;
	int ntime;
	int rtime;
	int begintime;
	int nresource;	//所需资源
	bool resstate;		//是否分配资源
	int reachtime;	//到达时间
	int ctime;				//提交时间
	int Qnum;			//进程所在队列编号
	struct jcb* link;
}*ready = NULL, *p, *finish = NULL, *noarrive = NULL;

typedef struct jcb JCB;

/*队列的顶点数组*/
struct VexNode
{
	int num;
	struct jcb *firstArc;
}*queen;

/*先来先服务*/
void FCFS()
{
	JCB *first, *second;
	int insert = 0;
	if ((ready == NULL) || ((p->reachtime) < (ready->reachtime))) /*最先到达者,插入队首*/
	{
		p->link = ready;
		ready = p;
	}
	else
	{
		first = ready;
		second = first->link;
		while (second != NULL)
		{
			if ((p->reachtime) < (second->reachtime))
			{
				p->link = second;
				first->link = p;
				second = NULL;
				insert = 1;
			}
			else
			{
				first = first->link;
				second = second->link;
			}
		}
		if (insert == 0) first->link = p;
	}
}


/* 建立进程控制块函数*/
void input(int choose)
{
	int i, num;
	int nowq = -1;
	system("cls"); /*清屏*/
	//printf("\n 请输入进程号:");
	//scanf("%d", &num);
	//printf("\n 请输入时间片大小");
	//scanf("%d", &timesize);
	num = 4;
	timesize = 3;
	queen = (VexNode *)calloc(N, sizeof(VexNode));
	for (int i = 0; i < N; i++)
	{
		queen[i].firstArc = NULL;
	}

	for (i = 0; i < num; i++)
	{
		//printf("\n 进程号No.%d:\n", i);
		//p = getpch(PCB);
		//printf("\n 输入进程名:");
		//scanf("%s", p->name);
		//printf("\n 输入进程优先数:");
		//scanf("%d", &p->super);
		//printf("\n 输入进程运行时间:");
		//scanf("%d", &p->ntime);
		p = getpch(JCB);
		printf("\n 输入进程名:");
		scanf("%s", p->name);
		printf("\n");
		p->super = i + 3;
		p->ntime = 1 + rand() % 3;
		p->nresource = 3 + rand() % 16;
		p->reachtime = i;
		p->Qnum = -1;
		p->begintime = 0;
		p->rtime = 0;
		p->state = 'W';
		p->link = NULL;
		p->ctime = 0;
		p->resstate = 0;
		switch (choose)
		{
		case m_FCFS:
		{
			FCFS();
		}
		break;
		case m_SJF:
		{
			if ((p->reachtime) > 0 && noarrive == NULL)
				noarrive = p;
			else if ((p->reachtime) > 0)		/*将尚未到达的作业按最短作业优先放入noarrive队列中*/
			{
				JCB *first, *second;
				int insert = 0;
				first = noarrive;
				second = first->link;
				while (second != NULL)
				{
					if ((p->reachtime) < (second->reachtime))
					{
						p->link = second;
						first->link = p;
						second = NULL;
						insert = 1;
					}
					else
					{
						first = first->link;
						second = second->link;
					}
				}
				if (insert == 0) first->link = p;
			}
			else if (p->reachtime == 0) ready = p;
		}
		break;
		default:
			break;
		}
	}
}

/*查看队列的长度，显示当前进程状态*/
int space()
{
	int l = 0; JCB* pr = ready;
	while (pr != NULL)
	{
		l++;
		pr = pr->link;
	}
	return(l);
}

void display()
{
	JCB *pr = finish;
	printf("\n\n 进程已经完成.\n\n");
	printf("\n 作业名 \t 状态 \t 服务时间 \t 开始时间 \t 完成时间 \t 周转时间 \t 带权周转时间  \n");
	int len = 0;
	double SUM[50] = { 0.0 }, temp = 0.0, temp1 = 0.0;
	while (pr != NULL)
	{
		printf("|%s\t\t", pr->name);
		printf("|%c\t\t", pr->state);
		printf("|%d\t\t", pr->ntime);
		printf("|%d\t\t", pr->begintime);
		printf("|%d\t\t", pr->ctime);
		printf("|%d\t\t", (pr->ctime) - (pr->reachtime));
		SUM[len] = (double)(((pr->ctime) - (pr->reachtime)) / (pr->ntime));
		printf("|%4.2f\t\t", SUM[len]);
		temp += SUM[len];
		temp1 += ((pr->ctime) - (pr->reachtime));
		len++;
		pr = pr->link;
		printf("\n");
	}
	printf("平均周转时间：%4.2f\n", temp1 / (len + 1));
	printf("带权平均周转时间：%4.2f\n", temp / (len + 1));
}

void destroy(JCB *pr)
{
	printf("\n 作业 [%s] 已完成.\n", p->name);
	p->state = 'F';
	Resourse += p->nresource;
	if (finish == NULL)
	{
		pr->link = NULL;
		finish = pr;
	}
	else
	{
		p->link = finish;
		finish = p;
	}
	p = ready;
}

void running()
{


	JCB *first, *second;
	first = p;
	second = p->link;
	while (first->state == 'R')
	{
		if (second != NULL && second->state == 'R')
		{
			(second->rtime)++;
			if ((second->rtime) == (second->ntime))
			{
				first->link = second->link;
				destroy(second);
				second = first->link;
				if (Resourse > (ready->ntime))
				{
					JCB *temp = ready;
					ready->link=
					first = ready;
					ready = ready->link;
					
				}
			}
			first = first->link;
			second = second->link;
		}
		else if ((second == NULL) && (first != NULL))
		{
			(first->rtime)++;
			if (first->rtime == first->ntime)
			{
				destroy(first);
			}
			first = first->link;
		}
		else if ((second->state !='R') && (first != NULL))
		{
			(first->rtime)++;
			if (first->rtime == first->ntime)
			{
				destroy(first);
				break;
			}
			first = first->link;
		}
	
	}

	while(temp->)

	//int insert = 0;
	//first = ready;
	//second = first->link;
	//while (second != NULL)
	//{
	//	if ((p->reachtime) < (second->reachtime))
	//	{
	//		p->link = second;
	//		first->link = p;
	//		second = NULL;
	//		insert = 1;
	//	}
	//	else
	//	{
	//		first = first->link;
	//		second = second->link;
	//	}
	//}
	//if (insert == 0) first->link = p;



}

int main()
{
	int len, h = 0, nowtime = 0;
	char ch;
	int choose = 1;
	int tag = 1;
	//Init();
	//scanf("%d", &choose);
	input(choose);
	len = space();
	while ((len != 0) && (ready != NULL))
	{
		p = ready;
		while (ready->state = 'R')		/*令ready指向未处理的作业*/
		{
			ready = ready->link;
		}
		running();		/*多道处理*/
			if ((noarrive != NULL) && (p != NULL))
			{
				if (nowtime = (noarrive->reachtime))
				{
					JCB *temp = p;
					if (ready->link != NULL)
					{
						ready = p->link;
						p = noarrive;
						temp->link = NULL;
						noarrive = noarrive->link;
						p->link = NULL;
						tag = 1;
						FCFS();

						p = ready;
						temp->link = p;
						p = temp;
						ready = temp;
					}
					else
					{
						p->link = noarrive;
						noarrive = noarrive->link;
						p->link->link = NULL;
					}
				}
			}
			else if (p == NULL)
			{
				tag = 0;		/*计算机空闲，没有进程需要调度*/
				if (noarrive == NULL) continue;
				if (nowtime = (noarrive->reachtime))
				{
					p = noarrive;
					ready = p;
					noarrive = noarrive->link;
					tag = 1;
				}
			}



	}
	printf("\n\n 进程已经完成.\n\n");
	display();
	free(queen);
	ch = getchar();
	system("pause");
	return 0;
}

