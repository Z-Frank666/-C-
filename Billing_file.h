#pragma once
#ifndef BILLING_FILE_H
#define BILLING_FILE_H
int saveBilling(const Billing* pBilling, const char* pPath);//�洢��Ϣ���ļ�
int readBilling(Billing* pBilling, const char* pPath);		//��ȡ�ļ�
int getBillingCount(const char* pPath);						//��ȡ��Ϣ����
int updataBilling(const Billing* pBilling, const char* pPath, int nIndex);		//��������
#endif