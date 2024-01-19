#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"model.h"
#include"global.h"
#include"tool.h"
#include<string.h>
#include<stdlib.h>

//����Ʒ���Ϣ
int saveBilling(const Billing* pBilling, const char* pPath)
{
	//��׷�ӷ�ʽ��������д��

	FILE* fp;
	fp = fopen(pPath, "ab+");

	if (fp == NULL)
	{
		printf("�ļ��޷���ȷ�򿪣�����д��Ʒ���Ϣ��\n");
		return 0;
	}
	//���Ʒ���Ϣ���浽�ļ���
	fwrite(pBilling, sizeof(*pBilling), 1, fp);
	//�ر��ļ�
	fclose(fp);
	printf("�Ʒ���Ϣ�ɹ������ļ��У�\n\n");
	return 1;
}

//���ļ���ͼ���ļ��ж�ȡ��Ϣ
int readBilling(Billing* pBilling, const char* pPath)
{
	int nIndex = 0;
	//�Զ����Ʒ�ʽ��ȡ
	FILE* fp;
	fp = fopen(pPath, "rb");
	if (fp == NULL)
	{
		printf("�ļ��޷���ȷ�򿪣����ܶ�ȡ�Ʒ���Ϣ��\n");
		return 0;
	}
	else {
		//�����ļ�����ȡ�ļ�����
		while (!feof(fp))
		{
			if (fread(&pBilling[nIndex], sizeof(Billing), 1, fp) != 0) {
				nIndex++;
			}
		}
		//�ر��ļ�
		fclose(fp);
		return 1;
	}
}

//��ȡ�ļ�����Ϣ����
int getBillingCount(const char* pPath)
{
	int nCount = 0;
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));
	//�Զ����Ʒ�ʽ��ȡ

	FILE* fp;
	fp = fopen(pPath, "rb");

	if (fp == NULL) {
		printf("�ļ��޷���ȷ�򿪣�����ͳ�ƼƷ���Ϣ������\n");
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
	//�ر��ļ�
	fclose(fp);
	free(pBilling);
	return nCount;
}

int updataBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
	int nCount = 0;		//��ȡ����
	long lPosition = 0;		//�ļ�λ��
	Billing pbBuf;

	//�Զ����ƶ�дģʽ���ļ������ʧ�ܣ�����FALSE
	FILE* fp;
	fp = fopen(pPath, "rb+");

	if (fp == NULL)
	{
		printf("�ļ��޷���ȷ�򿪣����ܸ���������Ϣ��\n");
		return FALSE;
	}
	//�����ļ�����ȡ������Ϣ���ļ��е�λ��
	while ((!feof(fp)) && (nCount < nIndex))
	{
		if (fread(&pbBuf, sizeof(Billing), 1, fp) != 0)
		{
			//��ȡ�ļ���ʶλ�ã����һ�����ҵ���λ��
			lPosition = ftell(fp);
		}
		nCount++;
	}
	//�ƶ����ļ���ʶλ��
	fseek(fp, lPosition - sizeof(Billing), 0);		//���ļ�ָ�����Ҫ�滻����β��ǰһ��

	//��������Ϣд���ļ�
	fwrite(pBilling, sizeof(Billing), 1, fp);
	printf("-----******-----�����ɹ���ллʹ�ã�-----******-----\n\n");

	fclose(fp);
	return TRUE;
}