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
	time_t Now = time(NULL);	//��ǰʱ��

	printf("��������Ҫͳ�Ƶ�����·�(�Խ���ʱ��Ϊ׼���ÿո����)��");
	scanf("%d %d", &nYear, &nMonth);
	system("cls");

	//�ж�����ʱ���Ƿ����Ҫ��
	if (nMonth > 12 || nMonth < 1 || nYear > timeToYear(Now))
	{
		printf("�������ͳ��ʱ�����\n");
		Sleep(1000);
		system("cls");
		return;
	}

	if (doStatisticMonth(nYear, nMonth, &fTotalUse) == FALSE) //����ֻ�ı䲢���ظ��·��е���Ӫҵ��
	{
		printf("ͳ��ʧ�ܣ�\n");
		system("pause");
		system("cls");
		return;
	}

	//���
	printf("��ѯ������Ϣ���£�\n%d��%d�µ���ʹ�ý��Ϊ��%5.2f\n", nYear, nMonth, fTotalUse);
	system("pause");
	system("cls");
	return;
}


//ͳ��һ���û��ϻ�����
void statisticUser()
{
	char aName[18];		//���ڴ���û����ƣ�
	char aStartTime[30] = { 0 };		//��ſ�ʼʱ��
	char aLastTime[30] = { 0 };		//��Ž���ʱ��
	float sum = 0;		//ͳ����Ŀ
	lpBillingNode node = NULL;
	printf("��������Ҫ��ѯ���û����ţ�");
	scanf("%s", aName);
	system("cls");

	if (FALSE == doStatisticUser(aName, &node))
	{
		printf("��ѯ����\n");
		system("pause");
		system("cls");
		return;
	}


	else
	{
		printf("����%s��ʹ����Ϣ���£�\n", aName);		//�����ͷ
		printf("--------------------------------------------------------------------------------------------\n");
		printf("�ϻ�ʱ��                      �»�ʱ��                      ʹ�ý��       �Ƿ����  �Ƿ�ɾ��\n");
		while (node->next != NULL)
		{
			node = node->next;
			timetostring(node->data.tStart, aStartTime);
			printf("%s", aStartTime);

			if (node->data.tEnd == 0) //���tEndΪ0��֤��δ�»���ִ�����δ�»�����
			{
				printf("δ�»�                        ");
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
		printf("\n�ÿ����ѽ��Ϊ%.2f\n", sum);
		system("pause");
		system("cls");
		return;
	}
}