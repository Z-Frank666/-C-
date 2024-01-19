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
		printf("�ļ��޷���ȷ�򿪣�����д��Ʒѱ�׼��Ϣ��\n");
		return 0;
	}
	fwrite(pstandard, sizeof(*pstandard), 1, fp);

	fclose(fp);
	printf("�µļƷѱ�׼�ɹ������ļ��У�\n\n");
	return TRUE;
}

int getstandardCount(const char* ppath)//ͳ�ƼƷѱ�׼����
{
	int nCount = 0;
	standard* pstandard = (standard*)malloc(sizeof(standard));
	//�Զ����Ʒ�ʽ��ȡ

	FILE* fp;
	fp = fopen(ppath, "rb");

	if (fp == NULL) {
		printf("�ļ��޷���ȷ�򿪣�����ͳ�ƼƷѱ�׼��Ϣ������\n");
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
	//�ر��ļ�
	fclose(fp);
	free(pstandard);
	return nCount;

}

int updatastandard(const standard* pstandard, const char* ppath, int nIndex)//���¼Ʒѱ�׼��Ϣ
{
	int nCount = 0;		//��ȡ����
	long lPosition = 0;		//�ļ�λ��
	standard pbBuf;

	//�Զ����ƶ�дģʽ���ļ������ʧ�ܣ�����FALSE
	FILE* fp;
	fp = fopen(ppath, "rb+");

	if (fp == NULL)
	{
		printf("�ļ��޷���ȷ�򿪣����ܸ��¼Ʒѱ�׼��Ϣ��\n");
		return FALSE;
	}
	//�����ļ�����ȡ������Ϣ���ļ��е�λ��
	while ((!feof(fp)) && (nCount < nIndex))
	{
		if (fread(&pbBuf, sizeof(standard), 1, fp) != 0)
		{
			//��ȡ�ļ���ʶλ�ã����һ�����ҵ���λ��
			lPosition = ftell(fp);
		}
		nCount++;
	}
	//�ƶ����ļ���ʶλ��
	fseek(fp, lPosition - sizeof(standard), 0);		//���ļ�ָ�����Ҫ�滻����β��ǰһ��

	//��������Ϣд���ļ�
	fwrite(pstandard, sizeof(standard), 1, fp);
	printf("-----******-----�����ɹ���ллʹ�ã�-----******-----\n\n");

	fclose(fp);
	return TRUE;
}


int readstandard(standard* pstandard, const char* ppath)
{
	int nIndex = 0;
	//�Զ����Ʒ�ʽ��ȡ
	FILE* fp;
	fp = fopen(ppath, "rb");
	if (fp == NULL)
	{
		printf("�ļ��޷���ȷ�򿪣����ܶ�ȡ�Ʒѱ�׼��Ϣ��\n");
		return 0;
	}
	else {
		//�����ļ�����ȡ�ļ�����
		while (!feof(fp))
		{
			if (fread(&pstandard[nIndex], sizeof(standard), 1, fp) != 0) {
				nIndex++;
			}
		}
		//�ر��ļ�
		fclose(fp);
		return 1;
	}
}