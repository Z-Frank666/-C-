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

struct Card acard[50];//定义结构体数组全局变量

int ncount = 0;//定义全局变量添加的个数

pcardnode cardlist = NULL;//定义一个全局临时链表单元

int getcard();

int std[50] = { 0 };

//初始化链表
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

//释放节点空间
void freecardlist()
{
	pcardnode cur;
	if (cardlist != NULL)
	{
		cur = cardlist->next;
		while (cur != NULL)//在临时链表不为空时，依次清空每个节点
		{
			cur = cardlist->next;
			free(cur);
			cur = NULL;
		}

	}
	else
		cur = NULL;

	free(cur);//最终清空所有节点
}

//将添加的卡信息添加到文件中
int addcard(struct Card card)
{

	return savecard(&card, CARDPATH);

}

//防止同用户名注册
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
//模糊查询实现
Card* querycard_1(const char* qname, int* p)
{
	pcardnode cur = NULL;

	Card* pcard = (Card*)malloc(sizeof(Card));

	if (FALSE == getcard())
	{
		return ;
	}//文件为空
	cur = cardlist->next;

	//遍历链表，节点为空则到达尾部 
	while (cur != NULL)
	{
		//遍历链表查找是否包含pname字符串
		if (strstr(cur->data.aName, qname) != NULL)//引入strstr函数，在由头至尾的链表卡号数据中查找所查询卡号，若找到，返回后者在前者中第一次出现的地址，否则返回NULL
		{
			pcard[*p] = cur->data;//将头节点数据赋值为所查卡号
			(*p)++;//若找到所查字符串，计数值由零加一
			pcard = (Card*)realloc(pcard, ((*p) + 1) * sizeof(Card));//更新链表空间，防止与循环语句冲突
		}
		cur = cur->next;
	}
	return pcard;
}

//精确查询实现
Card* querycard_2(const char* qname, int* p)
{
	pcardnode cur = NULL;

	Card* pcard = (Card*)malloc(sizeof(Card));

	if (FALSE == getcard())
	{
		return;
	}//文件为空
	cur = cardlist->next;

	//遍历链表，节点为空则到达尾部 
	while (cur != NULL)
	{
		//遍历链表查找是否包含pname字符串
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

//判断并修改上机状态
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo)
{

	pcardnode node = NULL;
	pcardnode head = NULL;
	time_t logintime;
	Billing billing;

	int nPosition = 0;		//计费信息在链表中的索引号
	//获取卡信息
	if (getcard() == FALSE)
	{
		return 0;
	}

	node = cardlist->next;
	head = node;

	//判断能否上机
	while (node != NULL)
	{
		if (strcmp(node->data.aName, pName) == 0 && strcmp(node->data.apwd, pPwd) == 0)
		{   //状态确认
			if (node->data.status == 1)
			{
				printf("\n该卡未下机\n");
				return 0;
			}
			else if (node->data.status == 2)
			{
				printf("\n该卡已注销\n若想继续使用请重新激活\n");
				return 0;
			}
			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n该卡已过期\n若需要退款请直接注销卡\n若想继续使用请重新激活\n");
				return 0;
			}
			//更新链表中的卡信息；
			node->data.status = 1;
			node->data.usecount++;//使用次数+1
			node->data.tLast = time(NULL);
			//更新卡文件信息
			if (updata(head, CARDPATH) == TRUE)
			{
				//添加消费记录-此时只进行简单的初始化

				strcpy(billing.aCardName, pName);		//上机卡号
				billing.tStart = time(NULL);			//上机时间
				billing.tEnd = 0;						//下机时间
				billing.nStatus = 0;					//消费状态				
				billing.fAmount = 0;					//消费金额
				billing.nDel = 0;						//删除标识

				//先将计费信息保存到文件中
				if (TRUE == saveBilling(&billing, BILLINGPATH))
				{
					//组装上机信息
					strcpy(pInfo->aCardName, pName);
					pInfo->fBalance = node->data.balance;
					pInfo->tLogon = billing.tStart;
					return TRUE;
				}
			}
		}
		node = node->next;
	}
	printf("\n卡号或密码输入错误\n");
	return 0;
}

//计算消费金额
double comsume(time_t start)
{
	double money = 0;		//消费金额
	int nCount = 0;		//上机的时间单元数，每个单元15分钟
	int Sec = 0;		//消费时间（单位：秒）
	int nMinutes = 0;		//消费时间（单位：分）
	time_t tEnd = time(NULL);		//结算时间为当前时间
	char name[20];			//标准
	standard* pstandard = NULL;
	int nPosition = 0;
	int num;
	float standard;

	//计算消费时常
	Sec = (int)(tEnd - start);
	nMinutes = Sec / 60;
	//计算消费的时间单元数

	nCount = (nMinutes / UNIT == 0) ? 1:  (nMinutes / UNIT);
	system("cls");
	Sleep(200);

	//标准选择

	showallstandard();
	printf("请输入使用的标准(序号)：");
	/*scanf("%s", name);*/
	scanf("%d",&num);
	/*pstandard = querystandard(name, &nPosition);*/
	pstandard = querystd(num);
	standard = pstandard->standardmoney;

	//计算消费金额
	money = nCount * standard;
	return money;
}

//下机条件
Card* doSettle(const char* pscardname, const char* psPwd, SettleInfo* pInfo)
{
	pcardnode node = NULL;
	pcardnode head = NULL;//在此定义的是全局卡信息节点，方便调用getcard
	time_t logintime;
	Billing *pBilling=NULL;
	double money = 0;

	int nPosition = 0;		//计费信息在链表中的索引号
	//获取卡信息
	if (getcard() == FALSE)
	{
		return 0;
	}

	node = cardlist->next;
	head = node;

	//判断能否下机
	while (node != NULL) 
	{
		if (strcmp(node->data.aName, pscardname) == 0 && strcmp(node->data.apwd, psPwd)==0) {
			if (node->data.status == 0)
			{
				printf("\n该卡未上机\n");
				return 0;
			}
			else if (node->data.status == 2)
			{
				printf("\n该卡已注销\n若想继续使用请重新激活\n");
				return 0;
			}
			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n该卡已过期\n若需要退款请直接注销卡\n若想继续使用请重新激活\n");
				return 0;
			}
			node->data.status = 0;
			//根据卡号，查询计费信息
			pBilling = queryBilling(pscardname, &nPosition);//在查询的同时获取索引值

			node->data.status = 0;
			logintime = node->data.tLast;

			//获取此次消费金额
			money = comsume(logintime);
			node->data.totaluse += money;
			node->data.balance = node->data.balance - money;

			//对余额进行判断，不足则需进行充值
			if (node->data.balance < 0)
			{
				printf("\n此次消费金额：%.2lf\n\n", money);
				printf("余额不足，请先进行充值");
				return 0;
			}
			else
				printf("\n此次消费金额：%.2lf\n\n", money);

			//更新计费信息
			pBilling->fAmount = (float)money;		//消费信息
			pBilling->nStatus = 1;					//状态，已结算
			pBilling->tEnd = time(NULL);			//下机时间

			//更新文件中的计费信息
			if (FALSE == updataBilling(pBilling, BILLINGPATH, nPosition))
			{
				return FALSE;
			}
			//组装下机信息
			strcpy(pInfo->aCardName, pscardname);	//卡号
			pInfo->fAmount = (float)money;		//消费金额
			pInfo->fBalance = pInfo->fBalance - money;		//余额
			pInfo->tStart = pBilling->tStart;		//上机时间
			pInfo->tEnd = time(NULL);		//下机时间

			if (updata(head, CARDPATH) == 1) {
				return &node->data;
			}
			
		}
		node = node->next;
	}
	printf("输入有误！无法下机！\n");
	return 0;
}


//将文件中的卡信息保存到链表
int getcard()
{
	int ncount = 0;

	int i = 0;

	Card* pcard = NULL;//创建链表

	pcardnode node = NULL;
	pcardnode cur = NULL;//创建尾结点

	if (cardlist != NULL)
	{
		freecardlist();//释放节点空间
	}
	incardlist();//初始化

	//获取卡信息数量
	ncount = getcardcount(CARDPATH);

	//动态分配内存
	pcard = (Card*)malloc(sizeof(Card) * ncount);
	if (pcard == NULL)
	{
		return FALSE;
	}//分配失败

	//获取卡信息
	if (FALSE == readcard(pcard, CARDPATH))
	{
		free(pcard);
		pcard = NULL;
		return FALSE;
	}
	for (i = 0, node = cardlist; i < ncount; i++)
	{
		//为节点分配内存
		cur = (pcardnode)malloc(sizeof(cardnode));
		//分配失败，则返回
		if (cur == NULL)
		{
			free(pcard);
			return FALSE;
		}
		//初始新的空间，赋值为零
		memset(cur, 0, sizeof(cardnode));

		//将信息保存到节点中

		cur->data = pcard[i];
		cur->next = NULL;

		//将节点添加到链表尾部

		node->next = cur;
		node = cur;
	}
	free(pcard);
	pcard = NULL;
	return TRUE;
}


//充值
Card* doAddMoney(const char* fcardname, const char* fpassword, double money)
{
	pcardnode node = NULL;
	pcardnode head = NULL;

	//获取卡信息
	if (getcard() == FALSE)
	{
		return NULL;
	}

	node = cardlist->next;
	head = node;
	while (node != NULL)                             //判断账号密码是否输入正确
	{
		if (strcmp(node->data.aName, fcardname) == 0 && strcmp(node->data.apwd, fpassword) == 0)
		{   //状态确认
			if (node->data.status == 2)
			{
				printf("\n该卡已注销\n若想继续使用请重新激活\n");
				return NULL;
			}

			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n该卡已过期\n若需要退款请直接注销卡\n若想继续使用请重新激活\n");
				return NULL;
			}
			//更新时间
			node->data.tLast = time(NULL);
			//更新链表卡信息
			node->data.balance = node->data.balance + money;
			//更新卡文件信息
			if (updata(head, CARDPATH) == TRUE)
				return &node->data;
		}
		node = node->next;
	}

	printf("卡号或密码输入错误\n");
	return NULL;
}


// 退费
Card* doRefunMoney(const char* fcardname, const char* fpassword, double money)
{
	pcardnode node = NULL;
	pcardnode head = NULL;

	//获取卡信息
	if (getcard() == 0)
		return NULL;
	node = cardlist->next;
	head = node;
	while (node != NULL)                            //判断账号密码是否输入正确
	{
		if (strcmp(node->data.aName, fcardname) == 0 && strcmp(node->data.apwd, fpassword) == 0)
		{   //状态确认
			if (node->data.status == 2)
			{
				printf("\n该卡已注销\n");
				return NULL;
			}
			node->data.tLast = time(NULL);
			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n该卡已过期\n若需要退款请直接注销卡\n若想继续使用请重新激活\n");
				return NULL;
			}
			if (node->data.balance < money)          //余额确认
			{
				printf("\n该卡余额不足\n");
				return NULL;
			}
			//更新链表卡信息
			node->data.balance = node->data.balance - money;
			//更新卡文件信息
			if (updata(head, CARDPATH) == TRUE)
				return &node->data;
		}
		node = node->next;
	}
	printf("\n卡号或密码输入错误\n");
	return NULL;
}



//注销卡
Card* doAnnul(const char* fcardname, const char* fpassword)
{
	pcardnode node = NULL;
	pcardnode head = NULL;

	//获取卡信息
	if (getcard() == FALSE)
	{
		return NULL;
	}
	node = cardlist->next;
	head = node;
	while (node != NULL)                           //判断账号密码是否输入正确
	{
		if (strcmp(node->data.aName, fcardname) == 0 && strcmp(node->data.apwd, fpassword) == 0)
		{   //状态确认
			if (node->data.status == 2)
			{
				printf("\n该卡已注销\n");
				return NULL;
			}
			node->data.tLast = time(NULL);
			if (node->data.tLast - node->data.tEnd >= 0)
			{
				printf("\n该卡已过期\n若需要退款请直接注销卡\n");
				return NULL;
			}
			if (node->data.status == 1)
			{
				printf("\n该卡正在上机，无法注销！\n");
				return NULL;
			}
			printf("\n该退余额：%.2lf\n", node->data.balance);
			//更新链表卡信息
			node->data.balance = 0.00;
			node->data.status = 2;
			//更新卡文件信息
			if (updata(head, CARDPATH) == TRUE)
				return &node->data;
		}
		node = node->next;
	}
	printf("\n卡号或密码输入错误\n");
	return NULL;
}

//重新激活卡
Card* doRenew(const char* fcardname, const char* fpassword)
{
	struct tm* stime;
	struct tm* etime;
	pcardnode node = NULL;;
	pcardnode head = NULL;
	//获取卡信息
	if (FALSE == getcard())
	{
		return NULL;
	}
	node = cardlist->next;
	head = node;
	while (node != NULL)
	{   //判断账号密码是否输入正确
		if (strcmp(node->data.aName, fcardname) == 0 && strcmp(node->data.apwd, fpassword) == 0)
		{
			node->data.tLast = node->data.tStart = time(NULL);
			if ((node->data.tLast - node->data.tEnd < 0))
			{
				if (node->data.status == 0 || node->data.status == 1)
				{
					printf("\n该卡仍能使用，不需要重新激活\n");
					return NULL;
				}
			}
			//更新链表卡信息
			node->data.tEnd = time(NULL);
			node->data.status = 0;
			stime = localtime(&node->data.tStart);
			etime = localtime(&node->data.tEnd);
			etime->tm_year = stime->tm_year + 1;
			node->data.tEnd = mktime(etime);

			//更新卡文件信息
			if (updata(head, CARDPATH) == TRUE)
				return &node->data;
		}
		node = node->next;
	}
	printf("\n卡号或密码输入错误\n");
	return NULL;
}