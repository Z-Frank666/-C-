#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"model.h"
#include"global.h"
#include"tool.h"
#include<string.h>
#include<stdlib.h>
#define CARD 256

Card  prasecard(char* pbuf);

//卡信息存储进文件

int savecard(const Card* pcard, const char* ppath)
{
	FILE* fp = NULL;
	//打开文件
	char starttime[TIME] = { 0 };
	char endtime[TIME] = { 0 };
	char lasttime[TIME] = { 0 };
	if ((fp = fopen(ppath, "a")) == NULL)
	{
		fp = fopen(ppath, "a");
		if (fp == NULL)
		{
			return FALSE;
		}
	}

	//时间转变字符串
	timetostring(pcard->tStart, starttime);
	timetostring(pcard->tEnd, endtime);
	timetostring(pcard->tLast, lasttime);

	//卡号密码开卡状态开卡时间截止时间累计金额最后使用时间使用次数

	fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n", pcard->aName, pcard->apwd, pcard->status, starttime, endtime, pcard->totaluse, lasttime, pcard->usecount, pcard->balance, pcard->del);

	fclose(fp);

	return TRUE;

}

int readcard(Card* pcard, const char* pPath)//将文件信息转入链表
{
	FILE* fp = NULL;
	char abuf[CARD] = { 0 };
	int i = 0;

	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		return FALSE;
	}

	while (!feof(fp))
	{
		memset(abuf, 0, CARD);
		if (fgets(abuf, CARD, fp) != NULL)
		{
			if (strlen(abuf) > 0)
			{
				pcard[i] = prasecard(abuf);
				i++;
			}
		}
	}
	fclose(fp);

	return TRUE;
}

//解析函数
Card  prasecard(char* pbuf)
{
	Card card;//保存解析出的卡信息

	const char* delims = "##";//字符串分隔符

	char flag[10][20] = { 0 };	//保存分割后的字符串

	int index = 0;				//flag字符数组的下标

	char* buf = NULL;

	char* str = NULL;

	buf = pbuf;//第一次的解析buf是解析用的字符串

	while ((str = strtok(pbuf, delims)) != NULL)//strtok凭借“##”分割字符串
	{
		strcpy(flag[index], str);
		pbuf = NULL;
		index++;
	}

	//分割后的内容

	strcpy(card.aName, flag[0]);
	strcpy(card.apwd, flag[1]);
	card.status = atoi(flag[2]);//atoi将字符串转为整数
	card.tStart = stringtotime(flag[3]);
	card.tEnd = stringtotime(flag[4]);
	card.totaluse = (float)atof(flag[5]);
	card.tLast = stringtotime(flag[6]);
	card.usecount = atoi(flag[7]);
	card.balance = (float)atof(flag[8]);//atof将字符串转为浮点数
	card.del = atoi(flag[9]);

	return card;

}

int getcardcount(const char* ppath)//获取用户数
{
	FILE* fp = NULL;
	int ncount = 0;

	char abuf[CARD] = { 0 };//定义数组读取文件信息

	// 打开文件
	fp = fopen(ppath, "r");//读入信息到fp

	if (fp == NULL)
	{
		return FALSE;

	}

	//查询文件
	while (!feof(fp))
	{
		memset(abuf, 0, CARD);//清空数组

		if (fgets(abuf, CARD, fp) != NULL)//fgets将fp中信息读入abuf字符数组中

		{
			if (strlen(abuf) > 0)
			{
				ncount++;
			}
		}
	}
	//关闭文件
	fclose(fp);

	return ncount;

}

int updata(pcardnode pcard, const char* ppath)
{
	FILE* fp = NULL;
	char abuf[CARD] = { 0 };
	char starttime[TIME] = { 0 };
	char endtime[TIME] = { 0 };
	char lasttime[TIME] = { 0 };

	int nline = 0;			//文件行数
	int lposition = 0;		//文件位置标志
	//将时间转化为字符串
	timetostring(pcard->data.tStart, starttime);
	timetostring(pcard->data.tEnd, endtime);
	timetostring(pcard->data.tLast, lasttime);

	//以只写方式打开文件
	fp = fopen(ppath, "w");

	if (fp == NULL)
	{
		return FALSE;
	}
	//遍历文件，找到该条记录
	while (pcard != NULL)
	{
		timetostring(pcard->data.tStart, starttime);
		timetostring(pcard->data.tEnd, endtime);
		timetostring(pcard->data.tLast, lasttime);

		//将数据写进文件
		fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s###%d##%.1f##%d\n", pcard->data.aName, pcard->data.apwd, pcard->data.status, starttime, endtime, pcard->data.totaluse, lasttime, pcard->data.usecount,
			pcard->data.balance, pcard->data.del);

		pcard = pcard->next;
	}


	//关闭文件
	fclose(fp);

	return TRUE;

}