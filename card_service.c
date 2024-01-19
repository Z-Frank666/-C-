#define _CRT_SECURE_NO_WARNINGS
#include"model.h"
#include<string.h>
#include<stdlib.h>
#include"card_file.h"
#include<time.h>
#include <windows.h>
#include"global.h"
#include"Billing_file.h"
#include"Billing_service.h"
#include"standardfile.h"
#include"standardservice.h"

double comsume(time_t start);

struct Card acard[50];//����ṹ������ȫ�ֱ���

int ncount = 0;//����ȫ�ֱ�����ӵĸ���

pcardnode cardlist = NULL;//����һ��ȫ����ʱ����Ԫ

int getcard();

int std[50] = { 0 };

//��ʼ������
int incardlist()
{
	pcardnode head = NULL;
	head = (pcardnode)malloc(sizeof(cardnode));
	if (head != NULL)
	{
		head->next = NULL;
		cardlist = head;

		return TRUE;
	}
	free(head);
	return FALSE;
}

//�ͷŽڵ�ռ�
void freecardlist()
{
	pcardnode cur;
	if (cardlist != NULL)
	{
		cur = cardlist->next;
		while (cur != NULL)//����ʱ����Ϊ��ʱ���������ÿ���ڵ�
		{
			cur = cardlist->next;
			free(cur);
			cur = NULL;
		}

	}
	else
		cur = NULL;

	free(cur);//����������нڵ�
}

//����ӵĿ���Ϣ��ӵ��ļ���
int addcard(struct Card card)
{

	return savecard(&card, CARDPATH);

}

//��ֹͬ�û���ע��
int nameRecheck(const char* pName)
{
	pcardnode cur = NULL;

	if (FALSE == getcard())
	{
		return FALSE;
	}

	cur = cardlist->next;

	while (cur != NULL)
	{
		if (strcmp(cur->data.aName, pName) == 0)
		{
			return TRUE;
		}
		cur = cur->next;
	}

	return FALSE;
}
//ģ����ѯʵ��
Card* querycard_1(const char* qname, int* p)
{
	pcardnode cur = NULL;

	Card* pcard = (Card*)malloc(sizeof(Card));

	if (FALSE == getcard())
	{
		return ;
	}//�ļ�Ϊ��
	cur = cardlist->next;

	//���������ڵ�Ϊ���򵽴�β�� 
	while (cur != NULL)
	{
		//������������Ƿ����pname�ַ���
		if (strstr(cur->data.aName, qname) != NULL)//����strstr����������ͷ��β�������������в�������ѯ���ţ����ҵ������غ�����ǰ���е�һ�γ��ֵĵ�ַ�����򷵻�NULL
		{
			pcard[*p] = cur->data;//��ͷ�ڵ����ݸ�ֵΪ���鿨��
			(*p)++;//���ҵ������ַ���������ֵ�����һ
			pcard = (Card*)realloc(pcard, ((*p) + 1) * sizeof(Card));//��������ռ䣬��ֹ��ѭ������ͻ
		}
		cur = cur->next;
	}
	return pcard;
}

//��ȷ��ѯʵ��
Card* querycard_2(const char* qname, int* p)
{
	pcardnode cur = NULL;

	Card* pcard = (Card*)malloc(sizeof(Card));

	if (FALSE == getcard())
	{
		return;
	}//�ļ�Ϊ��
	cur = cardlist->next;

	//���������ڵ�Ϊ���򵽴�β�� 
	while (cur != NULL)
	{
		//������������Ƿ����pname�ַ���
		if (strcmp(cur->data.aName,qname)==0)
		{
			pcard = cur;
			(*p) += 1;
			break;
		}
		cur = cur->next;
	}
	return pcard;
}

//�жϲ��޸��ϻ�״̬
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo)
{

	pcardnode node = NULL;
	pcardnode head = NULL;
	time_t logintime;
	Billing billing;

	int nPosition = 0;		//�Ʒ���Ϣ�������е�������
	//��ȡ����Ϣ
	if (getcard() == FALSE)
	{
		return 0;
	}

	node = cardlist->next;
	head = node;

	//�ж��ܷ��ϻ�
	while (node != NULL)
	{
		if (strcmp(node->data.aName, pName) == 0 && strcmp(node->data.apwd, pPwd) == 0)
		{   //״̬ȷ��
			if (node->data.status == 1)
			{
				printf("\n�ÿ�δ�»�\n");
				return 0;
			}
			else if (node->data.status == 2)
			{
				printf("\n�ÿ���ע��\n�������ʹ�������¼���\n");
				return 0;
			}
			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n�ÿ��ѹ���\n����Ҫ�˿���ֱ��ע����\n�������ʹ�������¼���\n");
				return 0;
			}
			//���������еĿ���Ϣ��
			node->data.status = 1;
			node->data.usecount++;//ʹ�ô���+1
			node->data.tLast = time(NULL);
			//���¿��ļ���Ϣ
			if (updata(head, CARDPATH) == TRUE)
			{
				//������Ѽ�¼-��ʱֻ���м򵥵ĳ�ʼ��

				strcpy(billing.aCardName, pName);		//�ϻ�����
				billing.tStart = time(NULL);			//�ϻ�ʱ��
				billing.tEnd = 0;						//�»�ʱ��
				billing.nStatus = 0;					//����״̬				
				billing.fAmount = 0;					//���ѽ��
				billing.nDel = 0;						//ɾ����ʶ

				//�Ƚ��Ʒ���Ϣ���浽�ļ���
				if (TRUE == saveBilling(&billing, BILLINGPATH))
				{
					//��װ�ϻ���Ϣ
					strcpy(pInfo->aCardName, pName);
					pInfo->fBalance = node->data.balance;
					pInfo->tLogon = billing.tStart;
					return TRUE;
				}
			}
		}
		node = node->next;
	}
	printf("\n���Ż������������\n");
	return 0;
}

//�������ѽ��
double comsume(time_t start)
{
	double money = 0;		//���ѽ��
	int nCount = 0;		//�ϻ���ʱ�䵥Ԫ����ÿ����Ԫ15����
	int Sec = 0;		//����ʱ�䣨��λ���룩
	int nMinutes = 0;		//����ʱ�䣨��λ���֣�
	time_t tEnd = time(NULL);		//����ʱ��Ϊ��ǰʱ��
	char name[20];			//��׼
	standard* pstandard = NULL;
	int nPosition = 0;
	int num;
	float standard;

	//��������ʱ��
	Sec = (int)(tEnd - start);
	nMinutes = Sec / 60;
	//�������ѵ�ʱ�䵥Ԫ��

	nCount = (nMinutes / UNIT == 0) ? 1:  (nMinutes / UNIT);
	system("cls");
	Sleep(200);

	//��׼ѡ��

	showallstandard();
	printf("������ʹ�õı�׼(���)��");
	/*scanf("%s", name);*/
	scanf("%d",&num);
	/*pstandard = querystandard(name, &nPosition);*/
	pstandard = querystd(num);
	standard = pstandard->standardmoney;

	//�������ѽ��
	money = nCount * standard;
	return money;
}

//�»�����
Card* doSettle(const char* pscardname, const char* psPwd, SettleInfo* pInfo)
{
	pcardnode node = NULL;
	pcardnode head = NULL;//�ڴ˶������ȫ�ֿ���Ϣ�ڵ㣬�������getcard
	time_t logintime;
	Billing *pBilling=NULL;
	double money = 0;

	int nPosition = 0;		//�Ʒ���Ϣ�������е�������
	//��ȡ����Ϣ
	if (getcard() == FALSE)
	{
		return 0;
	}

	node = cardlist->next;
	head = node;

	//�ж��ܷ��»�
	while (node != NULL) 
	{
		if (strcmp(node->data.aName, pscardname) == 0 && strcmp(node->data.apwd, psPwd)==0) {
			if (node->data.status == 0)
			{
				printf("\n�ÿ�δ�ϻ�\n");
				return 0;
			}
			else if (node->data.status == 2)
			{
				printf("\n�ÿ���ע��\n�������ʹ�������¼���\n");
				return 0;
			}
			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n�ÿ��ѹ���\n����Ҫ�˿���ֱ��ע����\n�������ʹ�������¼���\n");
				return 0;
			}
			node->data.status = 0;
			//���ݿ��ţ���ѯ�Ʒ���Ϣ
			pBilling = queryBilling(pscardname, &nPosition);//�ڲ�ѯ��ͬʱ��ȡ����ֵ

			node->data.status = 0;
			logintime = node->data.tLast;

			//��ȡ�˴����ѽ��
			money = comsume(logintime);
			node->data.totaluse += money;
			node->data.balance = node->data.balance - money;

			//���������жϣ�����������г�ֵ
			if (node->data.balance < 0)
			{
				printf("\n�˴����ѽ�%.2lf\n\n", money);
				printf("���㣬���Ƚ��г�ֵ");
				return 0;
			}
			else
				printf("\n�˴����ѽ�%.2lf\n\n", money);

			//���¼Ʒ���Ϣ
			pBilling->fAmount = (float)money;		//������Ϣ
			pBilling->nStatus = 1;					//״̬���ѽ���
			pBilling->tEnd = time(NULL);			//�»�ʱ��

			//�����ļ��еļƷ���Ϣ
			if (FALSE == updataBilling(pBilling, BILLINGPATH, nPosition))
			{
				return FALSE;
			}
			//��װ�»���Ϣ
			strcpy(pInfo->aCardName, pscardname);	//����
			pInfo->fAmount = (float)money;		//���ѽ��
			pInfo->fBalance = pInfo->fBalance - money;		//���
			pInfo->tStart = pBilling->tStart;		//�ϻ�ʱ��
			pInfo->tEnd = time(NULL);		//�»�ʱ��

			if (updata(head, CARDPATH) == 1) {
				return &node->data;
			}
			
		}
		node = node->next;
	}
	printf("���������޷��»���\n");
	return 0;
}


//���ļ��еĿ���Ϣ���浽����
int getcard()
{
	int ncount = 0;

	int i = 0;

	Card* pcard = NULL;//��������

	pcardnode node = NULL;
	pcardnode cur = NULL;//����β���

	if (cardlist != NULL)
	{
		freecardlist();//�ͷŽڵ�ռ�
	}
	incardlist();//��ʼ��

	//��ȡ����Ϣ����
	ncount = getcardcount(CARDPATH);

	//��̬�����ڴ�
	pcard = (Card*)malloc(sizeof(Card) * ncount);
	if (pcard == NULL)
	{
		return FALSE;
	}//����ʧ��

	//��ȡ����Ϣ
	if (FALSE == readcard(pcard, CARDPATH))
	{
		free(pcard);
		pcard = NULL;
		return FALSE;
	}
	for (i = 0, node = cardlist; i < ncount; i++)
	{
		//Ϊ�ڵ�����ڴ�
		cur = (pcardnode)malloc(sizeof(cardnode));
		//����ʧ�ܣ��򷵻�
		if (cur == NULL)
		{
			free(pcard);
			return FALSE;
		}
		//��ʼ�µĿռ䣬��ֵΪ��
		memset(cur, 0, sizeof(cardnode));

		//����Ϣ���浽�ڵ���

		cur->data = pcard[i];
		cur->next = NULL;

		//���ڵ���ӵ�����β��

		node->next = cur;
		node = cur;
	}
	free(pcard);
	pcard = NULL;
	return TRUE;
}


//��ֵ
Card* doAddMoney(const char* fcardname, const char* fpassword, double money)
{
	pcardnode node = NULL;
	pcardnode head = NULL;

	//��ȡ����Ϣ
	if (getcard() == FALSE)
	{
		return NULL;
	}

	node = cardlist->next;
	head = node;
	while (node != NULL)                             //�ж��˺������Ƿ�������ȷ
	{
		if (strcmp(node->data.aName, fcardname) == 0 && strcmp(node->data.apwd, fpassword) == 0)
		{   //״̬ȷ��
			if (node->data.status == 2)
			{
				printf("\n�ÿ���ע��\n�������ʹ�������¼���\n");
				return NULL;
			}

			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n�ÿ��ѹ���\n����Ҫ�˿���ֱ��ע����\n�������ʹ�������¼���\n");
				return NULL;
			}
			//����ʱ��
			node->data.tLast = time(NULL);
			//����������Ϣ
			node->data.balance = node->data.balance + money;
			//���¿��ļ���Ϣ
			if (updata(head, CARDPATH) == TRUE)
				return &node->data;
		}
		node = node->next;
	}

	printf("���Ż������������\n");
	return NULL;
}


// �˷�
Card* doRefunMoney(const char* fcardname, const char* fpassword, double money)
{
	pcardnode node = NULL;
	pcardnode head = NULL;

	//��ȡ����Ϣ
	if (getcard() == 0)
		return NULL;
	node = cardlist->next;
	head = node;
	while (node != NULL)                            //�ж��˺������Ƿ�������ȷ
	{
		if (strcmp(node->data.aName, fcardname) == 0 && strcmp(node->data.apwd, fpassword) == 0)
		{   //״̬ȷ��
			if (node->data.status == 2)
			{
				printf("\n�ÿ���ע��\n");
				return NULL;
			}
			node->data.tLast = time(NULL);
			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n�ÿ��ѹ���\n����Ҫ�˿���ֱ��ע����\n�������ʹ�������¼���\n");
				return NULL;
			}
			if (node->data.balance < money)          //���ȷ��
			{
				printf("\n�ÿ�����\n");
				return NULL;
			}
			//����������Ϣ
			node->data.balance = node->data.balance - money;
			//���¿��ļ���Ϣ
			if (updata(head, CARDPATH) == TRUE)
				return &node->data;
		}
		node = node->next;
	}
	printf("\n���Ż������������\n");
	return NULL;
}



//ע����
Card* doAnnul(const char* fcardname, const char* fpassword)
{
	pcardnode node = NULL;
	pcardnode head = NULL;

	//��ȡ����Ϣ
	if (getcard() == FALSE)
	{
		return NULL;
	}
	node = cardlist->next;
	head = node;
	while (node != NULL)                           //�ж��˺������Ƿ�������ȷ
	{
		if (strcmp(node->data.aName, fcardname) == 0 && strcmp(node->data.apwd, fpassword) == 0)
		{   //״̬ȷ��
			if (node->data.status == 2)
			{
				printf("\n�ÿ���ע��\n");
				return NULL;
			}
			node->data.tLast = time(NULL);
			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n�ÿ��ѹ���\n����Ҫ�˿���ֱ��ע����\n");
				return NULL;
			}
			if (node->data.status == 1)
			{
				printf("\n�ÿ������ϻ����޷�ע����\n");
				return NULL;
			}
			printf("\n������%.2lf\n", node->data.balance);
			//����������Ϣ
			node->data.balance = 0.00;
			node->data.status = 2;
			//���¿��ļ���Ϣ
			if (updata(head, CARDPATH) == TRUE)
				return &node->data;
		}
		node = node->next;
	}
	printf("\n���Ż������������\n");
	return NULL;
}

//���¼��
Card* doRenew(const char* fcardname, const char* fpassword)
{
	struct tm* stime;
	struct tm* etime;
	pcardnode node = NULL;;
	pcardnode head = NULL;
	//��ȡ����Ϣ
	if (FALSE == getcard())
	{
		return NULL;
	}
	node = cardlist->next;
	head = node;
	while (node != NULL)
	{   //�ж��˺������Ƿ�������ȷ
		if (strcmp(node->data.aName, fcardname) == 0 && strcmp(node->data.apwd, fpassword) == 0)
		{
			node->data.tLast = node->data.tStart = time(NULL);
			if ((node->data.tLast - node->data.tEnd < 0))
			{
				if (node->data.status == 0 || node->data.status == 1)
				{
					printf("\n�ÿ�����ʹ�ã�����Ҫ���¼���\n");
					return NULL;
				}
			}
			//����������Ϣ
			node->data.tEnd = time(NULL);
			node->data.status = 0;
			stime = localtime(&node->data.tStart);
			etime = localtime(&node->data.tEnd);
			etime->tm_year = stime->tm_year + 1;
			node->data.tEnd = mktime(etime);

			//���¿��ļ���Ϣ
			if (updata(head, CARDPATH) == TRUE)
				return &node->data;
		}
		node = node->next;
	}
	printf("\n���Ż������������\n");
	return NULL;
}