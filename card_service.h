#pragma once
#include"global.h"
#include"model.h"
#ifndef CARD_SEVICE_H
#define CARD_SEVICE_H
int addcard(struct Card card);													//������ӿ���Ϣ���ṹ������
int getcard();																	//�����Ҫ����Ϣ
void  freecardlist();
int nameRecheck(const char* pName);												//��ֹͬ�û���ע��
Card* querycard_1(const char* qname, int* p);										//ʹ�������ѯ��(ģ��)
Card* querycard_2(const char* qname, int* p);										//ʹ�������ѯ��(��ȷ)
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);
Card* doSettle(const char* pscardname, const char* psPwd, SettleInfo* pInfo);
Card* doAddMoney(const char* fcardname, const char* fpassword, double money);   //��ֵʵ��
Card* doRefunMoney(const char* fcardname, const char* fpassword, double money); //�˷�ʵ��
Card* doAnnul(const char* fcardname, const char* fpassword);
Card* doRenew(const char* fcardname, const char* fpassword);
#endif