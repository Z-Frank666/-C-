#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"model.h"
#include"global.h"
#include"tool.h"
#include<string.h>
#include<stdlib.h>

//保存计费信息
int saveBilling(const Billing* pBilling, const char* pPath)
{
	//以追加方式按二进制写入

	FILE* fp;
	fp = fopen(pPath, "ab+");

	if (fp == NULL)
	{
		printf("文件无法正确打开！不能写入计费信息！\n");
		return 0;
	}
	//将计费信息保存到文件中
	fwrite(pBilling, sizeof(*pBilling), 1, fp);
	//关闭文件
	fclose(fp);
	printf("计费信息成功存入文件中！\n\n");
	return 1;
}

//打开文件试图从文件中读取信息
int readBilling(Billing* pBilling, const char* pPath)
{
	int nIndex = 0;
	//以二进制方式读取
	FILE* fp;
	fp = fopen(pPath, "rb");
	if (fp == NULL)
	{
		printf("文件无法正确打开！不能读取计费信息！\n");
		return 0;
	}
	else {
		//遍历文件，读取文件内容
		while (!feof(fp))
		{
			if (fread(&pBilling[nIndex], sizeof(Billing), 1, fp) != 0) {
				nIndex++;
			}
		}
		//关闭文件
		fclose(fp);
		return 1;
	}
}

//获取文件中信息个数
int getBillingCount(const char* pPath)
{
	int nCount = 0;
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));
	//以二进制方式读取

	FILE* fp;
	fp = fopen(pPath, "rb");

	if (fp == NULL) {
		printf("文件无法正确打开！不能统计计费信息数量！\n");
		return 0;
	}
	else {
		while (!feof(fp))
		{
			if (fread(pBilling, sizeof(Billing), 1, fp) != 0) {
				nCount++;
			}
		}
	}
	//关闭文件
	fclose(fp);
	free(pBilling);
	return nCount;
}

int updataBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
	int nCount = 0;		//读取数量
	long lPosition = 0;		//文件位置
	Billing pbBuf;

	//以二进制读写模式打开文件，如果失败，返回FALSE
	FILE* fp;
	fp = fopen(pPath, "rb+");

	if (fp == NULL)
	{
		printf("文件无法正确打开！不能更新消费信息！\n");
		return FALSE;
	}
	//遍历文件，获取消费信息在文件中的位置
	while ((!feof(fp)) && (nCount < nIndex))
	{
		if (fread(&pbBuf, sizeof(Billing), 1, fp) != 0)
		{
			//获取文件标识位置，最后一次是找到的位置
			lPosition = ftell(fp);
		}
		nCount++;
	}
	//移动到文件标识位置
	fseek(fp, lPosition - sizeof(Billing), 0);		//将文件指针从需要替换的行尾向前一行

	//将更新信息写入文件
	fwrite(pBilling, sizeof(Billing), 1, fp);
	printf("-----******-----操作成功，谢谢使用！-----******-----\n\n");

	fclose(fp);
	return TRUE;
}