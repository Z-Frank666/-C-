#pragma once
#include"global.h"
#include"model.h"
#ifndef CARD_SEVICE_H
#define CARD_SEVICE_H
int addcard(struct Card card);													//保存添加卡信息到结构体数组
int getcard();																	//获得需要卡信息
void  freecardlist();
int nameRecheck(const char* pName);												//防止同用户名注册
Card* querycard_1(const char* qname, int* p);										//使用链表查询卡(模糊)
Card* querycard_2(const char* qname, int* p);										//使用链表查询卡(精确)
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);
Card* doSettle(const char* pscardname, const char* psPwd, SettleInfo* pInfo);
Card* doAddMoney(const char* fcardname, const char* fpassword, double money);   //充值实现
Card* doRefunMoney(const char* fcardname, const char* fpassword, double money); //退费实现
Card* doAnnul(const char* fcardname, const char* fpassword);
Card* doRenew(const char* fcardname, const char* fpassword);
#endif