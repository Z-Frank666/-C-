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

//添加消费信息
int addBilling(const char* pName, Billing* pBilling)
{
	//将消费信息写入结构体
	strcpy(pBilling->aCardName, pName);	//上机卡号
	pBilling->tStart = time(NULL);		//上机时间
	pBilling->tEnd = 0;		//下机时间，若未下机，则标注为零
	pBilling->fAmount = 0;		//消费金额
	pBilling->nStatus = 0;		//消费状态 0-未结算，1-结算
	pBilling->nDel = 0;		//删除状态 0-未删 1-已删

	strcpy(pBilling->aCardName, pName);

	if (FALSE == saveBilling(pBilling, BILLINGPATH))
	{
		return FALSE;

	}
	return TRUE;
}

//初始化信息记录链表
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

//释放链表空间
void releaseBillingList()
{
	lpBillingNode cur = BillingList;
	lpBillingNode next = NULL;
	//销毁链表
	while (cur != NULL)
	{
		next = cur->next;		//结点内存释放前，next保存其后继
		free(cur);
		cur = next;
	}
	BillingList = NULL;
}

//获取消费信息
int getBilling()
{
	int Billingcount = 0;

	int i = 0;

	Billing* pBilling = NULL;//创建链表

	lpBillingNode node = NULL;
	lpBillingNode cur = NULL;//创建尾结点

	if (BillingList != NULL)
	{
		releaseBillingList();
	}
	initBillingList();

	//获取卡信息数量
	Billingcount = getBillingCount(BILLINGPATH);

	//动态分配内存
	pBilling = (Billing*)malloc(sizeof(Card) * Billingcount);
	if (pBilling == NULL)
	{
		return FALSE;
	}//分配失败

	//获取卡信息
	if (FALSE == readBilling(pBilling, BILLINGPATH))
	{
		free(pBilling);
		pBilling = NULL;
		return FALSE;
	}
	for (i = 0, node = BillingList; i < Billingcount; i++)
	{
		//为节点分配内存
		cur = (lpBillingNode)malloc(sizeof(BillingNode));
		//分配失败，则返回
		if (cur == NULL)
		{
			free(pBilling);
			return FALSE;
		}
		//初始新的空间，赋值为零
		memset(cur, 0, sizeof(BillingNode));

		//将信息保存到节点中

		cur->data = pBilling[i];
		cur->next = NULL;

		//将节点添加到链表尾部

		node->next = cur;
		node = cur;
	}
	free(pBilling);
	pBilling = NULL;
	return TRUE;
}

//实现消费查询-从消费文件中读取消费信息
Billing* queryBilling(const char* pName, int* pIndex)
{
	lpBillingNode node = NULL;
	int nIndex = 0;

	//从信息文件读取信息列表
	if (getBilling() == FALSE)
	{
		return NULL;

	}

	//node指向第一个数据，开始遍历
	node = BillingList;//头数据

	//遍历链表。到NULL时停止
	while (node != NULL)
	{
		//确定信息与查找情况符合
		if (strcmp(node->data.aCardName, pName) == 0 && node->data.nStatus == 0)
		{
			//卡号正确并且没有结算信息
			return &node->data;
		}
		//移动下一个节点
		node = node->next;
		nIndex++;
		*pIndex = nIndex;//记录索引值
	}
	return NULL;
}

//对确定月份内数据进行统计
int doStatisticMonth(const int nYear, const int nMonth, float* fTotalUse)
{
	lpBillingNode node = NULL;
	*fTotalUse = 0.0;	//初始化金额
	char aName[18];		//用于存放用户名称；
	char aStartTime[30] = { 0 };		//存放开始时间
	char aLastTime[30] = { 0 };		//存放结束时间

	//从计费信息文件中读取计费信息列表，失败返回NULL
	if (getBilling() == FALSE)
	{
		return FALSE;
	}

	//node指向第一个数据，准被开始遍历
	node = BillingList;		//第一个数据
	printf("--------------------------------------------------------------------------------------------\n");
	printf("账号       上机时间                      下机时间                 使用金额 \n");
	while (node != NULL)
	{

		if ((node->data.nStatus == 1) && (timeToYear(node->data.tEnd) == nYear) && (timeToMonth(node->data.tEnd) == nMonth))
		{
			printf("%s", node->data.aCardName);
			//已缴费并且以最后下机时间来计算消费金额
			(*fTotalUse) = (*fTotalUse) + node->data.fAmount;

			//进行判断后输出每一条满足的信息
			timetostring(node->data.tStart, aStartTime);
			printf("%20s", aStartTime);

			if (node->data.tEnd == 0) //如果tEnd为0，证明未下机，执行输出未下机操作
			{
				printf("未下机                        ");
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

//统计一位用户的使用数据

int doStatisticUser(const char* aName, lpBillingNode* curHead)
{

	int i = 0;
	int nCount = 0;		//实际的计费信息数
	Billing* pBilling = NULL;		// 读取到的一条计费信息
	lpBillingNode node = NULL;		//当前的尾节点
	lpBillingNode cur = NULL;		//添加到链表中的节点

	//清空链表中已经存在的数据
	if (BillingList != NULL)
	{
		releaseBillingList();
	}
	//初始化链表
	initBillingList();

	//获取文件中计费信息个数
	nCount = getBillingCount(BILLINGPATH);

	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);

	if (pBilling != NULL)
	{
		//如果返回FALSE则读取失败
		if (readBilling(pBilling, BILLINGPATH) == FALSE)
		{
			free(pBilling);
			return FALSE;
		}

		//将读取的信息，保存到链表中
		for (i = 0, node = BillingList; i < nCount; i++)
		{
			if (strcmp(pBilling[i].aCardName, aName) == 0)
			{
				//为节点分配内存
				cur = (lpBillingNode)malloc(sizeof(BillingNode));
				//如果分配内存失败，则返回
				if (cur == NULL) {
					free(pBilling);
					return FALSE;
				}
				//初始化新的空间，全部赋值为0；
				memset(cur, 0, sizeof(BillingNode));

				//将数据保存到链表中
				cur->data = pBilling[i];
				cur->next = NULL;

				//将结点添加到链表的尾部
				node->next = cur;
				node = cur;
			}
		}
		*curHead = BillingList;
		//释放内存
		free(pBilling);
		return TRUE;
	}
	return FALSE;
}