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

/* ������̿��ƿ�JCB */
struct jcb { 
	char name[10];
	char state;
	int super;
	int ntime;
	int rtime;
	int begintime;
	int nresource;	//������Դ
	bool resstate;		//�Ƿ������Դ
	int reachtime;	//����ʱ��
	int ctime;				//�ύʱ��
	int Qnum;			//�������ڶ��б��
	struct jcb* link;
}*ready = NULL, *p,*finish=NULL,*noarrive=NULL;

typedef struct jcb JCB;

/*���еĶ�������*/
struct VexNode
{
	int num;
	struct jcb *firstArc;
}*queen;



void Init()
{
	printf("\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t\t---------------------------------\n");
	printf("\t\t\t\t\t\t| \t1.�Ƚ��ȳ��������㷨      |\n");
	printf("\t\t\t\t\t\t| \t2.�����ҵ�㷨          |\n");
	printf("\t\t\t\t\t\t---------------------------------\n");
	printf("\t\t\t\t\t\t           ������:");
}


/*�����ȷ���*/
void FCFS()
{
	JCB *first, *second;
	int insert = 0;
	if ((ready == NULL) || ((p->reachtime) < (ready->reachtime))) /*���ȵ�����,�������*/
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

/*����ʣ������ʱ��Ķ���ҵ�����㷨*/
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
//	else		/*�����ҵ����Ҫ����Դ���ڼ����������Դ������ҵ����ȴ��������*/
//	{
//		if ((ready == NULL) || (((p->ntime)-(p->rtime)) < ((ready->ntime)-(ready->rtime))))/*����ʱ����ٵ�������*/
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
//				if((((p->ntime) - (p->rtime)) < ((second->ntime) - (second->rtime))))	/*��������̱ȵ�ǰ����ʱ����,*/
//				{ /*���뵽��ǰ����ǰ��*/
//					p->link = second;
//					first->link = p;
//					second = NULL;
//					insert = 1;
//				}
//				else /* �������ʱ��������,����뵽��β*/
//				{
//					first = first->link;
//					second = second->link;
//				}
//			}
//			if (insert == 0) first->link = p;
//		}
//	}
//}

/*��������ʱ��Ķ���ҵ�����㷨*/
void SJF()
{
	JCB *first, *second;
	int insert = 0;

	if ((ready == NULL) || ((p->ntime) < (ready->ntime)))/*����ʱ����ٵ�������*/
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
			if ((p->ntime) < (second->ntime)) /*��������̱ȵ�ǰ����ʱ����,*/
			{ /*���뵽��ǰ����ǰ��*/
				p->link = second;
				first->link = p;
				second = NULL;
				insert = 1;
			}
			else /* �������ʱ��������,����뵽��β*/
			{
				first = first->link;
				second = second->link;
			}
		}
		if (insert == 0) first->link = p;
	}
}

/* �������̿��ƿ麯��*/
void input(int choose)
{
	int i, num;
	int nowq = -1;
	system("cls"); /*����*/
	//printf("\n ��������̺�:");
	//scanf("%d", &num);
	//printf("\n ������ʱ��Ƭ��С");
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
		//printf("\n ���̺�No.%d:\n", i);
		//p = getpch(PCB);
		//printf("\n ���������:");
		//scanf("%s", p->name);
		//printf("\n �������������:");
		//scanf("%d", &p->super);
		//printf("\n �����������ʱ��:");
		//scanf("%d", &p->ntime);
		p = getpch(JCB);
		printf("\n ���������:");
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
			else if((p->reachtime)>0)		/*����δ�������ҵ�������ҵ���ȷ���noarrive������*/
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

/*�鿴���еĳ��ȣ���ʾ��ǰ����״̬*/
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
	printf("\n\n �����Ѿ����.\n\n");
	printf("\n ��ҵ�� \t ״̬ \t ����ʱ�� \t ��ʼʱ�� \t ���ʱ�� \t ��תʱ�� \t ��Ȩ��תʱ��  \n");
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
		printf("ƽ����תʱ�䣺%4.2f\n", temp1 / (len+1));
		printf("��Ȩƽ����תʱ�䣺%4.2f\n", temp / (len + 1));
}

void disp(JCB * pr) /*����������ʾ����,������ʾ��ǰ����*/
{
	printf("\n qname \t state \t super \t ndtime \t runtime \n");
	printf("|%s\t\t", pr->name);
	printf("|%c\t\t", pr->state);
	printf("|%d\t\t", pr->super);
	printf("|%d\t\t", pr->ntime);
	printf("|%d\t\t", pr->rtime);
	printf("\n");
}

/* �������̲鿴���� */
void check()
{
	JCB* pr;

	printf("\n **** ��ǰ�������еĽ�����:%s", p->name); /*��ʾ��ǰ���н���*/
	disp(p);
	pr = ready;
	printf("\n ****��ǰ��������״̬Ϊ:\n"); /*��ʾ��������״̬*/
	while (pr != NULL)
	{
		disp(pr);
		pr = pr->link;
	}
}

/*�������̳�������(�������н���,��������)*/
void destroy()
{
	printf("\n ��ҵ [%s] �����.\n", p->name);
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

/*���������p��ready�Ѿ�������ˣ�*/
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

/* �������̾�������(��������ʱ�䵽,�þ���״̬)���Ӧ����ready���´ε����еĽ���*/
void running(int &nowtime, int choose)
{

		 (p->rtime)++;
		if (p->rtime == p->ntime)
		{
			if (p->ntime == 1) nowtime++;
			p->ctime = nowtime;
			if (ready != NULL && (ready->begintime == 0)) ready->begintime = nowtime + 1;
			destroy(); /* ����destroy����*/
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



int main() /*������*/
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
					bool fin = 0;			/*fin=1˵��ready�л���������Դ�ȼ������ԴС�������������*/
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
						/*$ ���Ƚ������*/
						display();
						system("pause");
						return 0;
					}
				}
				check();
				running(nowtime, choose);
		}
		if (choose = m_SJF)			/*δ�ﵽ����ҵ�Ĳ���*/
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
				tag = 0;		/*��������У�û�н�����Ҫ����*/
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
			//	tag = 0;		/*��������У�û�н�����Ҫ����*/
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

		printf("\n ����һ������......");
	//	ch = getchar();

	}
	printf("\n\n �����Ѿ����.\n\n");
	display();
	free(queen);
	ch = getchar();
	system("pause");
	return 0;
}



