#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<windows.h>
#include"tool.h"
#include"global.h"
#include"model.h"
#include"billing_service.h"
#include"statistic.h"


void statisticMonth()
{
	int nMonth, nYear;
	float fTotalUse = 0.0;
	time_t Now = time(NULL);	//当前时间

	printf("请输入需要统计的年份月份(以结束时间为准，用空格隔开)：");
	scanf("%d %d", &nYear, &nMonth);
	system("cls");

	//判断输入时间是否符合要求
	if (nMonth > 12 || nMonth < 1 || nYear > timeToYear(Now))
	{
		printf("输入的需统计时间错误！\n");
		Sleep(1000);
		system("cls");
		return;
	}

	if (doStatisticMonth(nYear, nMonth, &fTotalUse) == FALSE) //函数只改变并返回该月份中的总营业额
	{
		printf("统计失败！\n");
		system("pause");
		system("cls");
		return;
	}

	//输出
	printf("查询到的信息如下：\n%d年%d月的总使用金额为：%5.2f\n", nYear, nMonth, fTotalUse);
	system("pause");
	system("cls");
	return;
}


//统计一个用户上机数据
void statisticUser()
{
	char aName[18];		//用于存放用户名称；
	char aStartTime[30] = { 0 };		//存放开始时间
	char aLastTime[30] = { 0 };		//存放结束时间
	float sum = 0;		//统计数目
	lpBillingNode node = NULL;
	printf("请输入需要查询的用户卡号：");
	scanf("%s", aName);
	system("cls");

	if (FALSE == doStatisticUser(aName, &node))
	{
		printf("查询错误！\n");
		system("pause");
		system("cls");
		return;
	}


	else
	{
		printf("卡号%s的使用信息如下：\n", aName);		//输出表头
		printf("--------------------------------------------------------------------------------------------\n");
		printf("上机时间                      下机时间                      使用金额       是否结算  是否删除\n");
		while (node->next != NULL)
		{
			node = node->next;
			timetostring(node->data.tStart, aStartTime);
			printf("%s", aStartTime);

			if (node->data.tEnd == 0) //如果tEnd为0，证明未下机，执行输出未下机操作
			{
				printf("未下机                        ");
			}
			else
			{
				timetostring(node->data.tEnd, aLastTime);
				printf("%30s", aLastTime);
			}
			printf("%20.2f", node->data.fAmount);
			printf("%10d%10d\n", node->data.nStatus, node->data.nDel);

			sum = sum + node->data.fAmount;
		}
		printf("\n该卡消费金额为%.2f\n", sum);
		system("pause");
		system("cls");
		return;
	}
}