#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"model.h"
#include"global.h"
#include"tool.h"
#include<string.h>
#include<stdlib.h>
#define CARD 256

Card  prasecard(char* pbuf);

//����Ϣ�洢���ļ�

int savecard(const Card* pcard, const char* ppath)
{
	FILE* fp = NULL;
	//���ļ�
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

	//ʱ��ת���ַ���
	timetostring(pcard->tStart, starttime);
	timetostring(pcard->tEnd, endtime);
	timetostring(pcard->tLast, lasttime);

	//�������뿪��״̬����ʱ���ֹʱ���ۼƽ�����ʹ��ʱ��ʹ�ô���

	fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n", pcard->aName, pcard->apwd, pcard->status, starttime, endtime, pcard->totaluse, lasttime, pcard->usecount, pcard->balance, pcard->del);

	fclose(fp);

	return TRUE;

}

int readcard(Card* pcard, const char* pPath)//���ļ���Ϣת������
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

//��������
Card  prasecard(char* pbuf)
{
	Card card;//����������Ŀ���Ϣ

	const char* delims = "##";//�ַ����ָ���

	char flag[10][20] = { 0 };	//����ָ����ַ���

	int index = 0;				//flag�ַ�������±�

	char* buf = NULL;

	char* str = NULL;

	buf = pbuf;//��һ�εĽ���buf�ǽ����õ��ַ���

	while ((str = strtok(pbuf, delims)) != NULL)//strtokƾ�衰##���ָ��ַ���
	{
		strcpy(flag[index], str);
		pbuf = NULL;
		index++;
	}

	//�ָ�������

	strcpy(card.aName, flag[0]);
	strcpy(card.apwd, flag[1]);
	card.status = atoi(flag[2]);//atoi���ַ���תΪ����
	card.tStart = stringtotime(flag[3]);
	card.tEnd = stringtotime(flag[4]);
	card.totaluse = (float)atof(flag[5]);
	card.tLast = stringtotime(flag[6]);
	card.usecount = atoi(flag[7]);
	card.balance = (float)atof(flag[8]);//atof���ַ���תΪ������
	card.del = atoi(flag[9]);

	return card;

}

int getcardcount(const char* ppath)//��ȡ�û���
{
	FILE* fp = NULL;
	int ncount = 0;

	char abuf[CARD] = { 0 };//���������ȡ�ļ���Ϣ

	// ���ļ�
	fp = fopen(ppath, "r");//������Ϣ��fp

	if (fp == NULL)
	{
		return FALSE;

	}

	//��ѯ�ļ�
	while (!feof(fp))
	{
		memset(abuf, 0, CARD);//�������

		if (fgets(abuf, CARD, fp) != NULL)//fgets��fp����Ϣ����abuf�ַ�������

		{
			if (strlen(abuf) > 0)
			{
				ncount++;
			}
		}
	}
	//�ر��ļ�
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

	int nline = 0;			//�ļ�����
	int lposition = 0;		//�ļ�λ�ñ�־
	//��ʱ��ת��Ϊ�ַ���
	timetostring(pcard->data.tStart, starttime);
	timetostring(pcard->data.tEnd, endtime);
	timetostring(pcard->data.tLast, lasttime);

	//��ֻд��ʽ���ļ�
	fp = fopen(ppath, "w");

	if (fp == NULL)
	{
		return FALSE;
	}
	//�����ļ����ҵ�������¼
	while (pcard != NULL)
	{
		timetostring(pcard->data.tStart, starttime);
		timetostring(pcard->data.tEnd, endtime);
		timetostring(pcard->data.tLast, lasttime);

		//������д���ļ�
		fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s###%d##%.1f##%d\n", pcard->data.aName, pcard->data.apwd, pcard->data.status, starttime, endtime, pcard->data.totaluse, lasttime, pcard->data.usecount,
			pcard->data.balance, pcard->data.del);

		pcard = pcard->next;
	}


	//�ر��ļ�
	fclose(fp);

	return TRUE;

}