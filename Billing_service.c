#define _CRT_SECURE_NO_WARNINGS
#include"model.h"
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include"global.h"
#include"model.h"
#include"tool.h"
#include"Billing_file.h"

lpBillingNode BillingList = NULL;

//���������Ϣ
int addBilling(const char* pName, Billing* pBilling)
{
	//��������Ϣд��ṹ��
	strcpy(pBilling->aCardName, pName);	//�ϻ�����
	pBilling->tStart = time(NULL);		//�ϻ�ʱ��
	pBilling->tEnd = 0;		//�»�ʱ�䣬��δ�»������עΪ��
	pBilling->fAmount = 0;		//���ѽ��
	pBilling->nStatus = 0;		//����״̬ 0-δ���㣬1-����
	pBilling->nDel = 0;		//ɾ��״̬ 0-δɾ 1-��ɾ

	strcpy(pBilling->aCardName, pName);

	if (FALSE == saveBilling(pBilling, BILLINGPATH))
	{
		return FALSE;

	}
	return TRUE;
}

//��ʼ����Ϣ��¼����
void initBillingList()
{
	lpBillingNode head = NULL;
	if (BillingList == NULL)
	{
		head = (lpBillingNode)malloc(sizeof(BillingNode));
		head->next = NULL;
		BillingList = head;
	}
}

//�ͷ�����ռ�
void releaseBillingList()
{
	lpBillingNode cur = BillingList;
	lpBillingNode next = NULL;
	//��������
	while (cur != NULL)
	{
		next = cur->next;		//����ڴ��ͷ�ǰ��next��������
		free(cur);
		cur = next;
	}
	BillingList = NULL;
}

//��ȡ������Ϣ
int getBilling()
{
	int Billingcount = 0;

	int i = 0;

	Billing* pBilling = NULL;//��������

	lpBillingNode node = NULL;
	lpBillingNode cur = NULL;//����β���

	if (BillingList != NULL)
	{
		releaseBillingList();
	}
	initBillingList();

	//��ȡ����Ϣ����
	Billingcount = getBillingCount(BILLINGPATH);

	//��̬�����ڴ�
	pBilling = (Billing*)malloc(sizeof(Card) * Billingcount);
	if (pBilling == NULL)
	{
		return FALSE;
	}//����ʧ��

	//��ȡ����Ϣ
	if (FALSE == readBilling(pBilling, BILLINGPATH))
	{
		free(pBilling);
		pBilling = NULL;
		return FALSE;
	}
	for (i = 0, node = BillingList; i < Billingcount; i++)
	{
		//Ϊ�ڵ�����ڴ�
		cur = (lpBillingNode)malloc(sizeof(BillingNode));
		//����ʧ�ܣ��򷵻�
		if (cur == NULL)
		{
			free(pBilling);
			return FALSE;
		}
		//��ʼ�µĿռ䣬��ֵΪ��
		memset(cur, 0, sizeof(BillingNode));

		//����Ϣ���浽�ڵ���

		cur->data = pBilling[i];
		cur->next = NULL;

		//���ڵ���ӵ�����β��

		node->next = cur;
		node = cur;
	}
	free(pBilling);
	pBilling = NULL;
	return TRUE;
}

//ʵ�����Ѳ�ѯ-�������ļ��ж�ȡ������Ϣ
Billing* queryBilling(const char* pName, int* pIndex)
{
	lpBillingNode node = NULL;
	int nIndex = 0;

	//����Ϣ�ļ���ȡ��Ϣ�б�
	if (getBilling() == FALSE)
	{
		return NULL;

	}

	//nodeָ���һ�����ݣ���ʼ����
	node = BillingList;//ͷ����

	//����������NULLʱֹͣ
	while (node != NULL)
	{
		//ȷ����Ϣ������������
		if (strcmp(node->data.aCardName, pName) == 0 && node->data.nStatus == 0)
		{
			//������ȷ����û�н�����Ϣ
			return &node->data;
		}
		//�ƶ���һ���ڵ�
		node = node->next;
		nIndex++;
		*pIndex = nIndex;//��¼����ֵ
	}
	return NULL;
}

//��ȷ���·������ݽ���ͳ��
int doStatisticMonth(const int nYear, const int nMonth, float* fTotalUse)
{
	lpBillingNode node = NULL;
	*fTotalUse = 0.0;	//��ʼ�����
	char aName[18];		//���ڴ���û����ƣ�
	char aStartTime[30] = { 0 };		//��ſ�ʼʱ��
	char aLastTime[30] = { 0 };		//��Ž���ʱ��

	//�ӼƷ���Ϣ�ļ��ж�ȡ�Ʒ���Ϣ�б�ʧ�ܷ���NULL
	if (getBilling() == FALSE)
	{
		return FALSE;
	}

	//nodeָ���һ�����ݣ�׼����ʼ����
	node = BillingList;		//��һ������
	printf("--------------------------------------------------------------------------------------------\n");
	printf("�˺�       �ϻ�ʱ��                      �»�ʱ��                 ʹ�ý�� \n");
	while (node != NULL)
	{

		if ((node->data.nStatus == 1) && (timeToYear(node->data.tEnd) == nYear) && (timeToMonth(node->data.tEnd) == nMonth))
		{
			printf("%s", node->data.aCardName);
			//�ѽɷѲ���������»�ʱ�����������ѽ��
			(*fTotalUse) = (*fTotalUse) + node->data.fAmount;

			//�����жϺ����ÿһ���������Ϣ
			timetostring(node->data.tStart, aStartTime);
			printf("%20s", aStartTime);

			if (node->data.tEnd == 0) //���tEndΪ0��֤��δ�»���ִ�����δ�»�����
			{
				printf("δ�»�                        ");
			}
			else
			{
				timetostring(node->data.tEnd, aLastTime);
				printf("%30s", aLastTime);
			}
			printf("%20.2f\n", node->data.fAmount);


		}
		node = node->next;
	}
	return TRUE;
}

//ͳ��һλ�û���ʹ������

int doStatisticUser(const char* aName, lpBillingNode* curHead)
{

	int i = 0;
	int nCount = 0;		//ʵ�ʵļƷ���Ϣ��
	Billing* pBilling = NULL;		// ��ȡ����һ���Ʒ���Ϣ
	lpBillingNode node = NULL;		//��ǰ��β�ڵ�
	lpBillingNode cur = NULL;		//��ӵ������еĽڵ�

	//����������Ѿ����ڵ�����
	if (BillingList != NULL)
	{
		releaseBillingList();
	}
	//��ʼ������
	initBillingList();

	//��ȡ�ļ��мƷ���Ϣ����
	nCount = getBillingCount(BILLINGPATH);

	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);

	if (pBilling != NULL)
	{
		//�������FALSE���ȡʧ��
		if (readBilling(pBilling, BILLINGPATH) == FALSE)
		{
			free(pBilling);
			return FALSE;
		}

		//����ȡ����Ϣ�����浽������
		for (i = 0, node = BillingList; i < nCount; i++)
		{
			if (strcmp(pBilling[i].aCardName, aName) == 0)
			{
				//Ϊ�ڵ�����ڴ�
				cur = (lpBillingNode)malloc(sizeof(BillingNode));
				//��������ڴ�ʧ�ܣ��򷵻�
				if (cur == NULL) {
					free(pBilling);
					return FALSE;
				}
				//��ʼ���µĿռ䣬ȫ����ֵΪ0��
				memset(cur, 0, sizeof(BillingNode));

				//�����ݱ��浽������
				cur->data = pBilling[i];
				cur->next = NULL;

				//�������ӵ������β��
				node->next = cur;
				node = cur;
			}
		}
		*curHead = BillingList;
		//�ͷ��ڴ�
		free(pBilling);
		return TRUE;
	}
	return FALSE;
}