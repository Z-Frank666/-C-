#define _CRT_SECURE_NO_WARNINGS
#include"model.h"
#include<stdio.h>
#include<stdlib.h>
#include "global.h"

int savestandard(const standard* pstandard, const char* ppath)
{
	FILE* fp;
	fp = fopen(ppath, "ab+");

	if (fp == NULL)
	{
		printf("文件无法正确打开！不能写入计费标准信息！\n");
		return 0;
	}
	fwrite(pstandard, sizeof(*pstandard), 1, fp);

	fclose(fp);
	printf("新的计费标准成功存入文件中！\n\n");
	return TRUE;
}

int getstandardCount(const char* ppath)//统计计费标准条数
{
	int nCount = 0;
	standard* pstandard = (standard*)malloc(sizeof(standard));
	//以二进制方式读取

	FILE* fp;
	fp = fopen(ppath, "rb");

	if (fp == NULL) {
		printf("文件无法正确打开！不能统计计费标准信息数量！\n");
		return 0;
	}
	else {
		while (!feof(fp))
		{
			if (fread(pstandard, sizeof(standard), 1, fp) != 0) {
				nCount++;
			}
		}
	}
	//关闭文件
	fclose(fp);
	free(pstandard);
	return nCount;

}

int updatastandard(const standard* pstandard, const char* ppath, int nIndex)//更新计费标准信息
{
	int nCount = 0;		//读取数量
	long lPosition = 0;		//文件位置
	standard pbBuf;

	//以二进制读写模式打开文件，如果失败，返回FALSE
	FILE* fp;
	fp = fopen(ppath, "rb+");

	if (fp == NULL)
	{
		printf("文件无法正确打开！不能更新计费标准信息！\n");
		return FALSE;
	}
	//遍历文件，获取消费信息在文件中的位置
	while ((!feof(fp)) && (nCount < nIndex))
	{
		if (fread(&pbBuf, sizeof(standard), 1, fp) != 0)
		{
			//获取文件标识位置，最后一次是找到的位置
			lPosition = ftell(fp);
		}
		nCount++;
	}
	//移动到文件标识位置
	fseek(fp, lPosition - sizeof(standard), 0);		//将文件指针从需要替换的行尾向前一行

	//将更新信息写入文件
	fwrite(pstandard, sizeof(standard), 1, fp);
	printf("-----******-----操作成功，谢谢使用！-----******-----\n\n");

	fclose(fp);
	return TRUE;
}


int readstandard(standard* pstandard, const char* ppath)
{
	int nIndex = 0;
	//以二进制方式读取
	FILE* fp;
	fp = fopen(ppath, "rb");
	if (fp == NULL)
	{
		printf("文件无法正确打开！不能读取计费标准信息！\n");
		return 0;
	}
	else {
		//遍历文件，读取文件内容
		while (!feof(fp))
		{
			if (fread(&pstandard[nIndex], sizeof(standard), 1, fp) != 0) {
				nIndex++;
			}
		}
		//关闭文件
		fclose(fp);
		return 1;
	}
}