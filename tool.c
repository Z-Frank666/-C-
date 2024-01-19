#define _CRT_SECURE_NO_DEPRECATE
#include "tool.h"
#include<stdio.h>
#include<time.h>
#include"global.h"
#include<stdlib.h>

//��ʱ���ʽת��Ϊ������
void timetostring(time_t t, char* ptime)
{
	struct tm* pt;

	pt = localtime(&t);
	//����time_t���ͱ���
   //��ȡϵͳ���ں�ʱ��

	strftime(ptime, 20, "%Y-%m-%d %H:%M", pt);
	/*size_t strftime(char* str, size_t maxsize, const char* format, const struct tm* timeptr)
		���� format �ж���ĸ�ʽ�����򣬸�ʽ���ṹ timeptr ��ʾ��ʱ�䣬�������洢�� str �С�*/
}
//���ַ�������Ϊʱ������
time_t  stringtotime(char* pTime)
{
	struct tm tml;
	time_t   time1;

	if (sscanf(pTime, "%d-%d-%d %d:%d", &tml.tm_year, &tml.tm_mon, &tml.tm_mday, &tml.tm_hour, &tml.tm_min) == FALSE) {
		printf("��ȡ����");
	};
	/*int sscanf(const char* str, const char* format, ...)*/

	tml.tm_year -= 1900;  //���Ϊ��1900�꿪ʼ
	tml.tm_mon -= 1;      //�·�Ϊ1^11
	tml.tm_sec = 0;
	tml.tm_isdst -= -1;
	//��ԭϵͳʱ���������

	time1 = mktime(&tml);
	/*time_t mktime(struct tm* timeptr)*/
	//timeptr -- ����ָ���ʾ����ʱ��� time_t ֵ��ָ�룬������ʱ�䱻�ֽ�Ϊ���¸����֡������� timeptr �ṹ��ϸ�ڣ�
	//	struct tm {
	//	int tm_sec;         /* �룬��Χ�� 0 �� 59                */
	//	int tm_min;         /* �֣���Χ�� 0 �� 59                */
	//	int tm_hour;        /* Сʱ����Χ�� 0 �� 23                */
	//	int tm_mday;        /* һ���еĵڼ��죬��Χ�� 1 �� 31                    */
	//	int tm_mon;         /* �·ݣ���Χ�� 0 �� 11                */
	//	int tm_year;        /* �� 1900 �������                */
	//	int tm_wday;        /* һ���еĵڼ��죬��Χ�� 0 �� 6                */
	//	int tm_yday;        /* һ���еĵڼ��죬��Χ�� 0 �� 365                    */
	//	int tm_isdst;       /* ����ʱ                        */
	//};

	return time1;
}

//���time�е����
int timeToYear(time_t t)
{
	struct tm* timeinfo;
	timeinfo = (struct tm*)malloc(sizeof(struct tm));
	timeinfo = localtime(&t);
	return timeinfo->tm_year + 1900;
}


//[�õ�time_t����ʾ���·�
int timeToMonth(time_t t)
{
	struct tm* timeinfo;
	timeinfo = (struct tm*)malloc(sizeof(struct tm));
	timeinfo = localtime(&t);
	return timeinfo->tm_mon + 1;
}