#include "stdio.h" 
#include <stdlib.h> 
#include <conio.h> 
#include<string.h>

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
}*ready = NULL, *p,*finish=NULL,*noarrive=NULL;

typedef struct jcb JCB;

/*队列的顶点数组*/
struct VexNode
{
	int num;
	struct jcb *firstArc;
}*queen;



void Init()
{
	printf("\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t\t---------------------------------\n");
	printf("\t\t\t\t\t\t| \t1.先进先出单批道算法      |\n");
	printf("\t\t\t\t\t\t| \t2.最短作业算法          |\n");
	printf("\t\t\t\t\t\t---------------------------------\n");
	printf("\t\t\t\t\t\t           请输入:");
}


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

/*按照剩余运行时间的短作业优先算法*/
//void SJF()
//{
//	JCB *first, *second, *last;
//	int insert = 0;
//	if (p->nresource >Resourse)
//	{
//		last = ready;
//		if (last == NULL);
//		else
//		{
//			while (last->link != NULL)
//			{
//				last = last->link;
//			}
//		}
//		last->link = p;
//		p = ready;
//	}
//	else		/*如果作业所需要的资源大于计算机现有资源，将作业放入等待队列最后*/
//	{
//		if ((ready == NULL) || (((p->ntime)-(p->rtime)) < ((ready->ntime)-(ready->rtime))))/*所需时间从少到多排列*/
//		{
//			p->link = ready;
//			ready = p;
//		}
//		else
//		{
//			first = ready;
//			second = first->link;
//			while (second != NULL)
//			{
//				if((((p->ntime) - (p->rtime)) < ((second->ntime) - (second->rtime))))	/*若插入进程比当前进程时间少,*/
//				{ /*插入到当前进程前面*/
//					p->link = second;
//					first->link = p;
//					second = NULL;
//					insert = 1;
//				}
//				else /* 插入进程时间最多最低,则插入到队尾*/
//				{
//					first = first->link;
//					second = second->link;
//				}
//			}
//			if (insert == 0) first->link = p;
//		}
//	}
//}

/*按照所需时间的短作业优先算法*/
void SJF()
{
	JCB *first, *second;
	int insert = 0;

	if ((ready == NULL) || ((p->ntime) < (ready->ntime)))/*所需时间从少到多排列*/
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
			if ((p->ntime) < (second->ntime)) /*若插入进程比当前进程时间少,*/
			{ /*插入到当前进程前面*/
				p->link = second;
				first->link = p;
				second = NULL;
				insert = 1;
			}
			else /* 插入进程时间最多最低,则插入到队尾*/
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
			else if((p->reachtime)>0)		/*将尚未到达的作业按最短作业优先放入noarrive队列中*/
			{
				JCB *first, *second;
				int insert = 0;
				first = noarrive;
				second = first->link;
				while (second != NULL)
				{
					if ((p->ntime) < (second->ntime))
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
	double SUM[50] = { 0.0 }, temp = 0.0,temp1=0.0;
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
		printf("平均周转时间：%4.2f\n", temp1 / (len+1));
		printf("带权平均周转时间：%4.2f\n", temp / (len + 1));
}

void disp(JCB * pr) /*建立进程显示函数,用于显示当前进程*/
{
	printf("\n qname \t state \t super \t ndtime \t runtime \n");
	printf("|%s\t\t", pr->name);
	printf("|%c\t\t", pr->state);
	printf("|%d\t\t", pr->super);
	printf("|%d\t\t", pr->ntime);
	printf("|%d\t\t", pr->rtime);
	printf("\n");
}

/* 建立进程查看函数 */
void check()
{
	JCB* pr;

	printf("\n **** 当前正在运行的进程是:%s", p->name); /*显示当前运行进程*/
	disp(p);
	pr = ready;
	printf("\n ****当前就绪队列状态为:\n"); /*显示就绪队列状态*/
	while (pr != NULL)
	{
		disp(pr);
		pr = pr->link;
	}
}

/*建立进程撤消函数(进程运行结束,撤消进程)*/
void destroy()
{
	printf("\n 作业 [%s] 已完成.\n", p->name);
	p->state = 'F';
	Resourse += p->nresource;
	if (finish == NULL)
	{
		p->link = NULL;
		finish = p;
	}
	else
	{
		p->link = finish;
		finish = p;
	}
	p = ready;
}

/*多道管理，（p和ready已经排序好了）*/
void Multmanag()
{
	if ((ready!=NULL)&&ready == p)
	{
		if (ready->link!=NULL)
		{
			if ((ready->link->nresource) < Resourse&&(ready->link->nresource)==0)
			{
				Resourse -= ready->link->nresource;
				ready->link->resstate = 1;
				ready->link->state = 'R';
				ready = ready->link;
			}
			else return;
		}
		else return;
	}
	else if ((ready != NULL) && (ready == p->link))
	{
		if (ready->link != NULL)
		{
			if ((ready->nresource) < Resourse && (ready->nresource) == 0)
			{
				Resourse -= ready->nresource;
				ready->resstate = 1;
				ready->state = 'R';
				ready = ready->link;
			}
			else return;
		}
		else if (ready->link == NULL)
		{
			Resourse -= ready->nresource;
			ready->resstate = 1;
			ready->state = 'R';
			ready = NULL;
		}
	}
	else return;
}

/* 建立进程就绪函数(进程运行时间到,置就绪状态)结果应该是ready是下次的运行的进程*/
void running(int &nowtime, int choose)
{

		 (p->rtime)++;
		if (p->rtime == p->ntime)
		{
			if (p->ntime == 1) nowtime++;
			p->ctime = nowtime;
			if (ready != NULL && (ready->begintime == 0)) ready->begintime = nowtime + 1;
			destroy(); /* 调用destroy函数*/
		}
		else
		{
			(p->super)--;
			switch (choose)
			{
			case m_FCFS:
			{
				p->link = ready;
				ready = p;
			}
			break;
			case m_SJF:
			{
				p->link = ready;
				ready = p;
				//SJF();
			}
			break;
			default:
				break;
			}
		}
}



int main() /*主函数*/
{
	int len, h = 0, nowtime = 0;
	char ch;
	int choose = 2;
	int tag = 1;
	//Init();
	//scanf("%d", &choose);
	input(choose);
	len = space();
	while ((len != 0) && (ready != NULL))
	{
	//	ch = getchar();
		h++;
		nowtime++;
		if (tag)
		{
				printf("\n The execute number:%d \n", h);
				p = ready;
				ready = p->link;
				p->link = NULL;
				p->state = 'R';
				if (h % 50 == 0)
				{
					JCB *IsEmpty = ready;
					bool fin = 0;			/*fin=1说明ready中还有所需资源比计算机资源小，允许继续运行*/
					while (IsEmpty->link != NULL)
					{
						if (IsEmpty->nresource <= Resourse)
						{
							fin = 1;
							break;
						}
						else IsEmpty = IsEmpty->link;
					}
					if (fin == 0)
					{
						/*$ 调度结束语句*/
						display();
						system("pause");
						return 0;
					}
				}
				check();
				running(nowtime, choose);
		}
		if (choose = m_SJF)			/*未达到的作业的插入*/
		{
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
						SJF();
						p = ready;
						temp->link = p;
						p = temp;
						ready = temp;
					}
					else
					{
						p->link = noarrive;
						noarrive = noarrive->link;
						p->link->link=NULL;
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
			
			//if ((noarrive != NULL )&&(p!=NULL))
			//{
			//	ready = p;
			//	if (nowtime = (noarrive->reachtime))
			//	{
			//		if ((noarrive->ntime) < ((p->ntime) - (p->rtime)))
			//		{
			//			nowtime++;
			//			p->begintime = 0;
			//			noarrive->begintime = noarrive->reachtime + 1;
			//		}
			//		p = noarrive;
			//		noarrive = noarrive->link;
			//		p->link = NULL;
			//		tag = 1;
			//		SJF();
			//		p = ready;
			//	}
			//}
			//else if (p == NULL)
			//{
			//	tag = 0;		/*计算机空闲，没有进程需要调度*/
			//	if (noarrive == NULL) continue;
			//	if (nowtime = (noarrive->reachtime))
			//	{
			//		p = noarrive;
			//		p->begintime = nowtime;
			//		noarrive = noarrive->link;
			//		p->link = NULL;
			//		tag = 1;
			//		SJF();
			//		p = ready;
			//	}
			//}
		}

		printf("\n 按任一键继续......");
	//	ch = getchar();

	}
	printf("\n\n 进程已经完成.\n\n");
	display();
	free(queen);
	ch = getchar();
	system("pause");
	return 0;
}



