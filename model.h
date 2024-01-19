#pragma once
#include<time.h>
#ifndef MODEL_H
#define MODEL_H


typedef struct Card
{
	char aName[18];//����
	char apwd[8];//����
	int status;// ��״̬(0- δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ)
	float  totaluse; // �ۼƽ��
	int usecount; //ʹ�ô���
	float balance; //��� 
	int del; // ɾ����ʶ(0-δɾ����1-ɾ��)
	time_t tStart; // ����ʱ��
	time_t tEnd; //���Ľ�ֹʱ��
	time_t tLast; //���ʹ��ʱ��
}Card;

typedef struct cardnode//���忨����
{
	Card data;
	struct cardnode* next;


}cardnode, * pcardnode;


//������Ϣ�ṹ��
typedef struct Billing
{
	char aCardName[18];		//����		
	time_t tStart;		//�ϻ�ʱ��		
	time_t tEnd;		//�»�ʱ��		
	float fAmount;		//���ѽ��		
	int nStatus;		//����״̬��0��ʾδ���㣬1��ʾ�Ѿ�����		
	int nDel;		//ɾ��״̬��0��ʾδɾ����1��ʾ�Ѿ�ɾ��			
}Billing;

//�ϻ���Ϣ�ṹ��
typedef struct LogonInfo
{
	char aCardName[18];		//�ϻ�����
	time_t tLogon;		//�ϻ�ʱ��
	float fBalance;		//�ϻ�ʱ�����
}LogonInfo;

//�Ʒ���Ϣ���
typedef struct BillingNode
{
	Billing data;
	struct BillingNode* next;
}BillingNode, * lpBillingNode;


//�»���Ϣ�ṹ��
typedef struct SettleInfo
{
	char aCardName[18];		//����
	time_t tStart;		//�ϻ�ʱ��		
	time_t tEnd;		//�»�ʱ��		
	float fAmount;		//���ѽ��	
	float fBalance;		//���
}SettleInfo;

typedef struct standard
{
	char name[20];//�Ʒѱ�׼����
	float standardmoney;//�Ʒѱ�׼
	int status;//�Ʒѱ�׼״̬��0-���ã�1-��ɾ��

}standard;

//�Ʒѱ�׼��Ϣ����
typedef struct standardnode
{
	standard data;
	struct standardnode* next;
}standardnode, * lpstandardnode;


#endif