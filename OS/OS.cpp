// OS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>


#define getpch(type) (type*)malloc(sizeof(type)) 
#define NULL 0 
#define  N 5
#define m_MFQ 5
#define m_FCFS 4
#define m_RR 3
#define m_SJF 2
#define m_RF  1

int timesize;

	struct pcb { /* 定义进程控制块PCB */
		char name[10];
		char state;
		int super;
		int ntime;
		int rtime;
		int reachtime;	//到达时间
		int Qnum;			//进程所在队列编号
		struct pcb* link;
	}*ready = NULL, *p;
	
	typedef struct pcb PCB;

	/*队列的顶点数组*/
	struct VexNode
	{
		int num;
		struct pcb *firstArc;
	}*queen;

	void Init()
	{
		printf("\n\n\n\n\n\n\n\n");
		printf("\t\t\t\t\t\t---------------------------------\n");
		printf("\t\t\t\t\t\t| \t1.最优树优先算法        |\n");
		printf("\t\t\t\t\t\t| \t2.最短作业算法          |\n");
		printf("\t\t\t\t\t\t| \t3.轮转调度算法          |\n");
		printf("\t\t\t\t\t\t| \t4.先来先服务算法        |\n");
		printf("\t\t\t\t\t\t| \t5.多级反馈队列算法       |\n");
		printf("\t\t\t\t\t\t---------------------------------\n");
		printf("\t\t\t\t\t\t           请输入:");
	}

	/*先来先服务*/
	void FCFS()
	{
		PCB *first, *second;
		int insert = 0;
		if ((ready == NULL) || ((p->reachtime)< (ready->reachtime))) /*最先到达者,插入队首*/
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

	/*轮转调度算法,没有新开时间片*/
	void RR(int nowtime)
	{
		PCB *last;
		if (nowtime%timesize == 0)
		{
			last = ready;
			while (last->link != NULL)
			{/*使last指向队尾*/
				last = last->link;
			}
			last->link = p;
			p = ready;
		}
		else
		{
			p->link = ready;
			ready = p;
		}
	}

	/*短作业优先算法*/
	void SJF()
	{
		PCB *first, *second;
		int insert = 0;

		if ((ready == NULL) || ((p->ntime) <(ready->ntime)))/*所需时间从少到多排列*/
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

	/* 建立对进程进行优先级排列函数*/
	void sort() 
	{
		PCB *first, *second;
		int insert = 0;
		if ((ready == NULL) || ((p->super) > (ready->super))) /*优先级最大者,插入队首*/
		{
			p->link = ready;
			ready = p;
		}
		else /* 进程比较优先级,插入适当的位置中*/
		{
			first = ready;
			second = first->link;
			while (second != NULL)
			{
				if ((p->super) > (second->super)) /*若插入进程比当前进程优先数大,*/
				{ /*插入到当前进程前面*/
					p->link = second;
					first->link = p;
					second = NULL;
					insert = 1;
				}
				else /* 插入进程优先数最低,则插入到队尾*/
				{
					first = first->link;
					second = second->link;
				}
			}
			if (insert == 0) first->link = p;
		}
	}

	//返回不为空的最高优先数的队列编号
	int SearchRind(int nowq)
	{
		for (int i = 0; i < nowq; i++)
		{
			if (queen[i].firstArc != NULL)
			{
				return i;
			}
		}
	}

	/*多级(n级)反馈队列*/
	void MFQ(int &nowq,int nowtime)
	{
		if (nowtime%timesize == 0)
		{
			if (nowq == N - 1)
			{
				PCB *last;
				last = ready;
				if (last == NULL)		//只剩一个元素
				{
					ready = p;
					return;
				}
				else
				{
					while (last->link != NULL)
					{/*使last指向队尾*/
						last = last->link;
					}
					if (last != p)				/*如果剩下不止一个元素*/
					{
						last->link = p;
						p = ready;
					}
				}
			}
			else
			{
				PCB *last;
				int Hnum;			/*最大非空优先数的队列编号*/
				last = queen[nowq + 1].firstArc;
				if (last != NULL)   /*下一个队列为空则直接插入p到队首*/
				{
					while (last->link != NULL)
					{
						last = last->link;
					}
					last->link = p;
					p->Qnum++;
				}
				else
				{
					queen[nowq + 1].firstArc = p;
					p->Qnum++;
				}

				if (ready != NULL)			
				{
					if (p->Qnum == 0) ;
					else queen[nowq].firstArc = ready;		/*队首元素变成ready*/
				}
				else					/*  切换队列 */
				{
					if(p->Qnum==0) ready= queen[nowq+1].firstArc;
					else
					{
						queen[nowq].firstArc = NULL;
						ready = queen[++nowq].firstArc;
					}
					
				}
				p = ready;
			}
		}
		else
		{
		p->link = ready;
		ready = p;
		}
	}

	/* 建立进程控制块函数*/
	void input(int choose)
	{
		int i, num;
		int nowq = -1;
		system("cls"); /*清屏*/
		printf("\n 请输入进程数:");
		scanf("%d", &num);
		printf("\n 请输入时间片大小");
		scanf("%d", &timesize);
		num = 4;
		timesize = 3;

		queen = (VexNode *)calloc(N, sizeof(VexNode));
		for (int i=0; i < N; i++)
		{
			queen[i].firstArc = NULL;
		}

		for (i = 0; i < num; i++)
		{
			p = getpch(PCB);
			printf("\n 进程号No.%d:\n", i);
			printf("\n 输入进程名:");
			scanf("%s", p->name);
			printf("\n 输入进程优先数:");
			scanf("%d", &p->super);
			printf("\n 输入进程运行时间:");
			scanf("%d", &p->ntime);
			
			//p->super = i + 3;
			//p->ntime=8+rand()%16;
			p->reachtime = i;
			p->Qnum = -1;
			printf("\n");
			p->rtime = 0; p->state = 'w';
			p->link = NULL;
			switch (choose)
			{
			case m_RF:
			{
				sort(); /* 调用sort函数*/
			}
				break;
			case m_SJF:
			{
				SJF();
			}
				break;
			case m_FCFS:
			{
				FCFS();
			}
				break;
			case m_RR:
			{
				sort();
				//RR(0);
			}
				break;
			case m_MFQ:
			{
				MFQ(nowq,0);
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
		int l = 0; PCB* pr = ready;
		while (pr != NULL)
		{
			l++;
			pr = pr->link;
		}
		return(l);
	}

	void disp(PCB * pr) /*建立进程显示函数,用于显示当前进程*/
	{
		printf("\n qname \t state \t super \t ndtime \t runtime \n");
		printf("|%s\t", pr->name);
		printf("|%c\t", pr->state);
		printf("|%d\t", pr->super);
		printf("|%d\t", pr->ntime);
		printf("|%d\t", pr->rtime);
		printf("\n");
	}


	/* 建立进程查看函数 */
	void check() 
	{
		PCB* pr;
		
		printf("\n **** 当前正在运行的进程是:%s", p->name); /*显示当前运行进程*/
		disp(p);
		pr = ready;
		printf("\n ****当前就绪队列状态为:\n"); /*显示就绪队列状态*/
		while (pr != NULL)
		{
			disp(pr);
			pr = pr->link;
		}
		if (queen == NULL);
		else
		{
			PCB *print;
			for (int i = 0; i < N; i++)
			{
					print = queen[i].firstArc;
					pr = ready;
					printf("\n****第%d队列的进程\n", i);
					while (print!=NULL)
					{
						if(print->Qnum==i) printf("\t%s", print->name);
						print = print->link;
					}
					while (pr != NULL)
					{
						if (pr->Qnum == i) printf("\t%s",pr->name);
						pr = pr->link;
					}
					printf("\n");
			}	
		}

	}

	/*建立进程撤消函数(进程运行结束,撤消进程)*/
	void destroy() 
	{
		printf("\n 进程 [%s] 已完成.\n", p->name);
		free(p);
	}

	/* 建立进程就绪函数(进程运行时间到,置就绪状态)结果应该是ready是下次的运行的进程*/
	void running(int &nowtime,int choose,int &nowq)
	{
		(p->rtime)++;
		if (p->rtime == p->ntime)
		{
			if (choose == m_MFQ)									
			{																		
				queen[nowq].firstArc = ready;
				if ((nowq<N-1)&&ready == NULL && queen[nowq + 1].firstArc != NULL)		/*nowq队列为空，检查下一编号队列*/
				{
					ready = queen[nowq + 1].firstArc;
					nowq++;
				}
			}
			destroy(); /* 调用destroy函数*/
			nowtime = 0;
		}
		else
		{
			(p->super)--;
			p->state = 'w';
			switch (choose)
			{
			case m_FCFS:
				{
					FCFS();
				}
				break;
				case  m_SJF:
				{
					SJF();
				}
				break;
				case m_RR:
				{
					RR(nowtime);
				}
				break;
				case m_RF:
				{
					sort();
				}
				break;
				case m_MFQ:
				{
					MFQ(nowq,nowtime);
				}
				break;
			default:
				break;
			}
		}
	}

	int main() /*主函数*/
	{
		int len, h = 0,nowtime=0;
		char ch;
		int choose = 1;
		int nowq =0;
		Init();
		scanf("%d", &choose);
		input(choose);
		len = space();
		while ((len != 0) && (ready != NULL))
		{
		//	ch = getchar();
			h++;
			nowtime++;
			printf("\n The execute number:%d \n", h);
			p = ready;
			ready = p->link;		
			p->link = NULL;
			p->state ='R';
			check();
			running(nowtime,choose,nowq);
			printf("\n 按任一键继续......");
		//	ch = getchar();
		}
		printf("\n\n 进程已经完成.\n");
		system("pause");
		free(queen);
		ch = getchar();
	}




