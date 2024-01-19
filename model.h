#pragma once
#include<time.h>
#ifndef MODEL_H
#define MODEL_H


typedef struct Card
{
	char aName[18];//卡号
	char apwd[8];//密码
	int status;// 卡状态(0- 未上机；1-正在上机；2-已注销；3-失效)
	float  totaluse; // 累计金额
	int usecount; //使用次数
	float balance; //余额 
	int del; // 删除标识(0-未删除，1-删除)
	time_t tStart; // 开卡时间
	time_t tEnd; //卡的截止时间
	time_t tLast; //最后使用时间
}Card;

typedef struct cardnode//定义卡链表
{
	Card data;
	struct cardnode* next;


}cardnode, * pcardnode;


//消费信息结构体
typedef struct Billing
{
	char aCardName[18];		//卡号		
	time_t tStart;		//上机时间		
	time_t tEnd;		//下机时间		
	float fAmount;		//消费金额		
	int nStatus;		//消费状态，0表示未结算，1表示已经结算		
	int nDel;		//删除状态，0表示未删除，1表示已经删除			
}Billing;

//上机信息结构体
typedef struct LogonInfo
{
	char aCardName[18];		//上机卡号
	time_t tLogon;		//上机时间
	float fBalance;		//上机时卡余额
}LogonInfo;

//计费信息结点
typedef struct BillingNode
{
	Billing data;
	struct BillingNode* next;
}BillingNode, * lpBillingNode;


//下机信息结构体
typedef struct SettleInfo
{
	char aCardName[18];		//卡号
	time_t tStart;		//上机时间		
	time_t tEnd;		//下机时间		
	float fAmount;		//消费金额	
	float fBalance;		//余额
}SettleInfo;

typedef struct standard
{
	char name[20];//计费标准名字
	float standardmoney;//计费标准
	int status;//计费标准状态（0-可用，1-已删）

}standard;

//计费标准信息链表
typedef struct standardnode
{
	standard data;
	struct standardnode* next;
}standardnode, * lpstandardnode;


#endif