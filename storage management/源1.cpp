#include"pch.h"
#include<iostream>


using namespace std;

#define Free 0 //����״̬
#define Busy 1 //����״̬
#define OK 1    //���
#define ERROR 0 //����
#define MAX_length 640 //����ڴ�ռ�Ϊ640KB
typedef int Status;
int flag;

typedef struct freearea//����һ��������˵����ṹ
{
	long size;   //������С
	long address; //������ַ
	int state;   //״̬
}ElemType;

// ���Ա��˫������洢�ṹ
typedef struct DuLNode
{
	ElemType data;
	struct DuLNode *prior; //ǰ��ָ��
	struct DuLNode *next;  //���ָ��
}

DuLNode, *DuLinkList;
DuLinkList block_first; //ͷ���
DuLinkList block_last;  //β���
Status alloc(int);//�ڴ����
Status free(int); //�ڴ����
Status First_fit(int);//�״���Ӧ�㷨
Status Best_fit(int); //�����Ӧ�㷨
Status Worst_fit(int); //�����Ӧ�㷨
void show();//�鿴����
Status Initblock();//�����ռ��

Status Initblock()//������ͷ�����ڴ�ռ�����
{
	block_first = (DuLinkList)malloc(sizeof(DuLNode));
	block_last = (DuLinkList)malloc(sizeof(DuLNode));
	block_first->prior = NULL;
	block_first->next = block_last;
	block_last->prior = block_first;
	block_last->next = NULL;
	block_last->data.address = 0;
	block_last->data.size = MAX_length;
	block_last->data.state = Free;
	return OK;
}

//��������
Status alloc(int ch)
{
	int request = 0;
	cout << "��������Ҫ����������С(��λ:KB)��";
	cin >> request;
	if (request < 0 || request == 0)
	{
		cout << "�����С�����ʣ������ԣ�" << endl;
		return ERROR;
	}

	if (ch == 2) //ѡ�������Ӧ�㷨
	{
		if (Best_fit(request) == OK) cout << "����ɹ���" << endl;
		else cout << "�ڴ治�㣬����ʧ�ܣ�" << endl;
		return OK;
	}
	if (ch == 3) //ѡ�������Ӧ�㷨
	{
		if (Worst_fit(request) == OK) cout << "����ɹ���" << endl;
		else cout << "�ڴ治�㣬����ʧ�ܣ�" << endl;
		return OK;
	}
	else //Ĭ���״���Ӧ�㷨
	{
		if (First_fit(request) == OK) cout << "����ɹ���" << endl;
		else cout << "�ڴ治�㣬����ʧ�ܣ�" << endl;
		return OK;
	}
}

//�״���Ӧ�㷨
Status First_fit(int request)
{
	//Ϊ������ҵ�����¿ռ��ҳ�ʼ��
	DuLinkList temp = (DuLinkList)malloc(sizeof(DuLNode));
	temp->data.size = request;
	temp->data.state = Busy;

	DuLNode *p = block_first->next;
	while (p)
	{
		if (p->data.state == Free && p->data.size == request)
		{//�д�Сǡ�ú��ʵĿ��п�
			p->data.state = Busy;
			return OK;
			break;
		}
		if (p->data.state == Free && p->data.size > request)
		{//�п��п���������������ʣ��
			temp->prior = p->prior;
			temp->next = p;
			temp->data.address = p->data.address;
			p->prior->next = temp;
			p->prior = temp;
			p->data.address = temp->data.address + temp->data.size;
			p->data.size -= request;
			return OK;
			break;
		}
		p = p->next;
	}
	return ERROR;
}

//�����Ӧ�㷨
Status Best_fit(int request)
{
	int ch; //��¼��Сʣ��ռ�
	DuLinkList temp = (DuLinkList)malloc(sizeof(DuLNode));
	temp->data.size = request;
	temp->data.state = Busy;
	DuLNode *p = block_first->next;
	DuLNode *q = NULL; //��¼��Ѳ���λ��

	while (p) //��ʼ����С�ռ�����λ��
	{
		if (p->data.state == Free && (p->data.size >= request))
		{
			if (q == NULL)
			{
				q = p;
				ch = p->data.size - request;
			}
			else if (q->data.size > p->data.size)
			{
				q = p;
				ch = p->data.size - request;
			}
		}
		p = p->next;
	}

	if (q == NULL) return ERROR;//û���ҵ����п�
	else if (q->data.size == request)
	{
		q->data.state = Busy;
		return OK;
	}
	else
	{
		temp->prior = q->prior;
		temp->next = q;
		temp->data.address = q->data.address;
		q->prior->next = temp;
		q->prior = temp;
		q->data.address += request;
		q->data.size = ch;
		return OK;
	}
	return OK;
}

//�����Ӧ�㷨
Status Worst_fit(int request)
{
	int ch; //��¼���ʣ��ռ�
	DuLinkList temp = (DuLinkList)malloc(sizeof(DuLNode));
	temp->data.size = request;
	temp->data.state = Busy;
	DuLNode *p = block_first->next;
	DuLNode *q = NULL; //��¼��Ѳ���λ��

	while (p) //��ʼ�����ռ�����λ��
	{
		if (p->data.state == Free && (p->data.size >= request))
		{
			if (q == NULL)
			{
				q = p;
				ch = p->data.size - request;
			}
			else if (q->data.size < p->data.size)	//�ҵ����
			{
				q = p;
				ch = p->data.size - request;
			}
		}
		p = p->next;
	}

	if (q == NULL) return ERROR;//û���ҵ����п�
	else if (q->data.size == request)
	{
		q->data.state = Busy;
		return OK;
	}
	else
	{
		temp->prior = q->prior;
		temp->next = q;
		temp->data.address = q->data.address;
		q->prior->next = temp;
		q->prior = temp;
		q->data.address += request;
		q->data.size = ch;
		return OK;
	}
	return OK;
}

//�������
Status free(int flag)
{
	DuLNode *p = block_first;
	for (int i = 0; i <= flag; i++)
		if (p != NULL)
			p = p->next;
		else
			return ERROR;

	p->data.state = Free;
	if (p->prior != block_first && p->prior->data.state == Free)//��ǰ��Ŀ��п�����
	{
		p->prior->data.size += p->data.size;
		p->prior->next = p->next;
		p->next->prior = p->prior;
		p = p->prior;
	}
	if (p->next != block_last && p->next->data.state == Free)//�����Ŀ��п�����
	{
		p->data.size += p->next->data.size;
		if (p->next->next == NULL) p->next->prior = p;
		else p->next->next->prior = p;
		p->next = p->next->next;
	}
	if (p->next == block_last && p->next->data.state == Free)//�����Ŀ��п�����
	{
		p->data.size += p->next->data.size;
		p->next = NULL;
	}

	return OK;
}

//��ʾ����������
void show()
{
	int flag = 0;
	cout << "\n����������:\n";
	cout << "++++++++++++++++++++++++++++++++++++++++++++++\n\n";
	DuLNode *p = block_first->next;
	cout << "������\t��ʼ��ַ\t������С\t״̬\n\n";
	while (p)
	{
		cout << "  " << flag++ << "\t";
		cout << "  " << p->data.address << "\t\t";
		cout << " " << p->data.size << "KB\t\t";
		if (p->data.state == Free) cout << "����\n\n";
		else cout << "�ѷ���\n\n";
		p = p->next;
	}
	cout << "++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

//������
int main()
{
	int ch;//�㷨ѡ����
	cout << "��������ʹ�õ��ڴ�����㷨��\n";
	cout << "(1)�״���Ӧ�㷨\n(2)�����Ӧ�㷨\n(3)�����Ӧ�㷨\n";

	cin >> ch;
	while (ch < 1 || ch>3)
	{
		cout << "�������������������ʹ�õ��ڴ�����㷨��\n";
		cin >> ch;
	}

	Initblock(); //�����ռ��
	int choice;  //����ѡ����

	while (1)
	{
		show();
		cout << "���������Ĳ�����";
		cout << "\n1: �����ڴ�\n2: �����ڴ�\n0: �˳�\n";

		cin >> choice;
		if (choice == 1) alloc(ch); // �����ڴ�
		else if (choice == 2)  // �ڴ����
		{
			int flag;
			cout << "��������Ҫ�ͷŵķ����ţ�";
			cin >> flag;
			free(flag);
		}
		else if (choice == 0) break; //�˳�
		else //�����������
		{
			cout << "�������������ԣ�" << endl;
			continue;
		}
	}
	return 0;
}
