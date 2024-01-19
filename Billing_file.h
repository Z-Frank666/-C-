#pragma once
#ifndef BILLING_FILE_H
#define BILLING_FILE_H
int saveBilling(const Billing* pBilling, const char* pPath);//存储信息到文件
int readBilling(Billing* pBilling, const char* pPath);		//读取文件
int getBillingCount(const char* pPath);						//获取信息条数
int updataBilling(const Billing* pBilling, const char* pPath, int nIndex);		//更新链表
#endif