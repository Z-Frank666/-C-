#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include"tool.h"
#include"model.h"
#include"menu.h"
#include"card_service.h"
#include"Billing_service.h"
#include"statistic.h"
#include"standardfile.h"
#include"standardservice.h"

const char password[14] = { "0122210870531" };

void outputMenu()//主菜单
{

	printf("欢迎进入计费管理系统\n");
	printf("----------菜单----------\n");
	printf("     【1】添加卡 \n");
	Sleep(30);
	printf("     【2】查询卡\n");
	Sleep(30);
	printf("     【3】上机\n");
	Sleep(30);
	printf("     【4】下机\n");
	Sleep(30);
	printf("     【5】充值\n");
	Sleep(30);
	printf("     【6】退费\n");
	Sleep(30);
	printf("     【7】注销卡\n");
	Sleep(30);
	printf("     【8】管理员操作\n");
	Sleep(30);
	printf("     【0】退出\n\n");
	printf("请选择菜单项编号(0-8):\n");

}

//添加卡
void add()
{
	struct Card card;

	char aName[50] = { '\0' };
	char apwd[20] = { '\0' };

	//初始化时间
	struct  tm* startTime;//开卡时间
	struct tm* endTime;//截止时间
	//开卡时间，截止时间和最后使用时间都默认为当前的系统时间
	card.tStart = card.tEnd = card.tLast = time(NULL);
	startTime = localtime(&card.tStart);
	endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	card.tEnd = mktime(endTime);//将时间转化为time_t类型

	printf("请输入卡号<长度为0~18>:");

	scanf("%s", aName, 50);

	if (nameRecheck(aName) == TRUE)
	{
		printf("账号已注册，请重新注册");
		return;
	}
	//账号放入结构体
	strcpy(card.aName, aName);

	printf("\n请输入密码<长度为0~8>：");

	scanf("%s", apwd, 20);
	strcpy(card.apwd, apwd);

	//检查是否超出
	if (strlen(aName) > 18 || strlen(apwd) > 8) {
		printf("账号或密码输入格式错误\n");

		return;
	}
	
	printf("\n请输入开卡金额:");

	scanf("%f", &card.balance);

	card.totaluse = 0;

	card.usecount = 0;

	card.status = 0;

	card.del = 0;

	//输出卡状态
	printf("\n");

	printf("----------添加卡信息如下-------\n");

	printf("卡号\t密码\t卡状态\t余额\t\n");

	printf("%s\t%s\t%d\t%0.1f\t\n", card.aName, card.apwd, card.status, card.balance);

	//将新卡输入文件
	addcard(card);


}

void query()//查询卡的初始函数
{
	int select = 0;

	struct Card* qcard = NULL;

	char aName[18] = { 0 };
	char aTime[20] = { 0 };
	int n = 0;//判断是否查到

	printf("请选择你的查询方式：\n");
	printf("【1】    模糊查询\n");
	printf("【2】    精确查询\n");


	printf("你的选择：");

	scanf("%d", &select);

	printf("\n");

	switch (select)
	{
	case 1:
		printf("请输入查询卡号：");
		scanf("%s", aName);
		qcard = querycard_1(aName, &n);//此时qcard的值应为所查卡
		break;
	case 2:
		printf("请输入查询卡号：");
		scanf("%s", aName);
		qcard = querycard_2(aName, &n);//此时qcard的值应为所查卡
		break;
	}

	if (qcard == NULL || n == 0)
	{
		printf("没有查询到卡的信息");
	}

	int i = 0;//方便模糊查询遍历

	if(n)//查询成功时，n值应为非零(若选择精确查询，n必为1)
	{
		//输出查询到的卡
		printf("卡号\t卡状态\t余额\t累计使用\t使用次数\t上次使用时间\t\n");

		while (i < n) {

			timetostring(qcard[i].tLast, aTime);

			printf("%s\t%d\t%0.1f\t%0.1f\t\t%d\t\t%s\n", qcard[i].aName, qcard[i].status, qcard[i].balance, qcard[i].totaluse, qcard[i].usecount, aTime);

			i++;
		}
	}
}


//上机
void logon()
{
	char aName[18] = { 0 };
	char apwd[8] = { 0 };
	Card* pcard = NULL;//创建一个卡信息
	char lasttime[TIME] = { 0 };//定义一个字符数组存放上机时间
	LogonInfo* pInfo = NULL;//创建上机信息
	int nResult = 0;//判断是否能上机的参数

	memset(aName, 0, sizeof(aName));
	memset(apwd, 0, sizeof(apwd));
	memset(lasttime, 0, sizeof(lasttime));

	//提示并且接收用户输入卡号和密码
	printf("请输入上机卡号（长度为1-17）：");
	scanf("%s", aName);

	//判断输入的卡号是否符合要求
	if (strlen(aName) >= 18)
	{
		printf("输入卡号长度超过最大值！\n");

	}

	//提示并接受上机密码；
	printf("请输入上机密码（长度为1-7）:");
	scanf("%s", apwd);


	//判断输入的卡号密码是否符合要求
	if (strlen(apwd) >= 8)
	{
		printf("输入卡号长度超过最大值！\n");
	}

	//开始上机，获取上机信息
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));

	//根据上机的结果，提示输出不同信息
	nResult = doLogon(aName, apwd, pInfo);//判断上机情况
	switch (nResult)
	{
	case 0:
		printf("------------上机失败！------------\n"); break;
	case 1:
		printf("-----******-----上机信息如下-----******-----\n");
		printf("卡号              余额              上机时间\n");		//18 18

		timetostring(pInfo->tLogon, lasttime);

		//输出信息
		printf("%s", pInfo->aCardName);
		printf("%20.2f", pInfo->fBalance);
		printf("%30s\n", lasttime);
		printf("上机成功！\n");
		break;
	case 2:
		printf("------------该卡不能使用！------------\n"); break;
	case 3:
		printf("------------余额不足！------------\n"); break;
	}
	free(pInfo);
}

//下机实现
void settle()
{
	Card* scard = NULL;
	SettleInfo* pInfo = NULL;
	char tlogout[20] = { '\0' };
	char scardname[20] = { '\0' };
	char spwd[10] = { '\0' };
	printf("请输入下机卡号：");
	scanf("%s",scardname);
	printf("\n");
	printf("请输入下机卡密码：");
	scanf("%s",spwd);

	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	//更改下机状态
	scard = doSettle(scardname,spwd,pInfo);

	if (scard == NULL)
		printf("\n下机失败!\n");
	else
	{
		printf("卡号：%s\t", scard->aName);
		printf("余额：%.2f\t", scard->balance);
		timetostring(pInfo->tEnd, tlogout);
		printf("下机时间：%s\t", tlogout);
		printf("\n下机成功!\n");
	}

}

//充值
void addMoney()
{
	Card* acard = NULL;
	double money;
	char fcardname[18] = { '\0' };
	char fpassword[8] = { '\0' };
	printf("请输入卡号<长度为1-18>：");
	scanf("%s", &fcardname);           //接收卡号和密码
	printf("请输入密码<长度为1-8>：");
	scanf("%s", &fpassword);
	printf("请输入充值金额<RMB>：");	//该循环用来解决输入字符(串)后会进入死循环的问题，与main函数中用法相同
	while (scanf("%lf", &money) != 1)
	{
		printf("您输入的充值金额有误。(请输入整数或小数)\n");
		printf("请重新输入:\n");
		while (getchar() != '\n');
	}

	acard = doAddMoney(fcardname, fpassword, money);

	if (acard == NULL)
		printf("\n充值失败\n");
	else
	{
		printf("\n充值成功\n");
		printf("卡号：%s\n", acard->aName);
		printf("余额：%.2f\n", acard->balance);
	}
}

//退费
void refundMoney() {
	Card* acard = NULL;
	double money;
	char fcardname[18] = { '\0' };
	char fpassword[8] = { '\0' };
	printf("请输入卡号<长度为1-18>：");
	scanf("%s", &fcardname);
	printf("请输入密码<长度为1-8>：");
	scanf("%s", &fpassword);
	printf("请输入退费金额<RMB>：");
	//该循环用来解决输入字符(串)后会进入死循环的问题，与main函数中用法相同
	while (scanf("%lf", &money) != 1) {
		printf("您输入的退费金额有误。(请输入整数或小数)\n");
		printf("请重新输入:\n");
		while (getchar() != '\n');
	}

	acard = doRefunMoney(fcardname, fpassword, money);
	if (acard == NULL)
		printf("\n退费失败\n");
	else
	{
		printf("\n退费成功\n");
		printf("卡号：%s\n", acard->aName);
		printf("余额：%.2f\n", acard->balance);
	}

}

//注销卡
void annul()
{
	Card* acard = NULL;
	int n = 0;
	char fcardname[18] = { '\0' };
	char fpassword[8] = { '\0' };
	printf("请输入卡号<长度为1-18>：");
	scanf("%s", &fcardname);
	printf("请输入密码<长度为1-8>：");
	scanf("%s", &fpassword);

	acard = doAnnul(fcardname, fpassword);
	if (acard == NULL)
		printf("\n注销失败\n");
	else
		printf("\n注销成功\n");


}

void showmenu()
{
	printf("----------菜单----------\n");
	printf("     【1】查询统计 \n\n");
	Sleep(30);
	printf("     【2】激活卡\n\n");
	Sleep(30);
	printf("     【3】计费标准\n\n");
	Sleep(30);
	printf("     【0】退出\n\n");

}

//重新激活卡
void renew()
{
	Card* acard = NULL;
	char tend[20] = { '\0' };
	int n = 0;
	char fcardname[18] = { '\0' };
	char fpassword[8] = { '\0' };
	printf("请输入卡号<长度为1-18>：");
	scanf("%s", &fcardname);
	printf("请输入密码<长度为1-8>：");
	scanf("%s", &fpassword);
	acard = doRenew(fcardname, fpassword);
	if (acard == NULL)
		printf("\n重新激活失败\n");
	else
	{
		printf("\n重新激活成功\n");
		printf("卡号：%s\n", acard->aName);
		timetostring(acard->tEnd, tend);
		/*printf("卡截止时间已更改为：%s\n", tend);*/
	}
}

//显示所有计费标准信息
void allstandard()
{
	printf("所有的卡信息如下\n");
	showallstandard();
}


//计费管理系统标准选择的显示
void showguanli()
{
	printf("欢迎进入计费标准管理\n\n");
	printf("1.新增标准\n");
	printf("2.查询标准\n");
	printf("3.修改标准\n");
	/*printf("4.删除标准\n");*/
	printf("0.退出\n\n");
}

//对计费标准进行操作

void guanli()
{
	char* name;
	int choose = 0;
	standard* p1standard;
	standard* p2standard;
	int nPosition = 0;

	p1standard = (standard*)malloc(sizeof(standard));
	showguanli();

	printf("请输入你的选择(0--3):");

	scanf("%d", &choose);
	system("cls");
	Sleep(200);
	switch (choose)
	{
	case 1:
		addstandard(p1standard);
		break;
	case 2:
		allstandard();
		break;
	case 3:
		changestd();
		break;
	/*case 4:
		deletestandard();
		break;*/
	case 0:
		printf("欢迎下次使用");
		return;
	default:
		printf("请输入正确的编号");
		break;
	}
}

//统计
void statistic()
{
	int nSelection = -1;
	printf("（1）.按月份统计用户使用总营业额\n");
	printf("（2）.按用户卡号统计该用户使用金额\n");
	printf("（0）.返回\n");
	printf("请选择需要进行的统计操作：");

	//读取用户操作
	scanf("%d", &nSelection);
	system("cls");

	switch (nSelection)
	{
	case 0:
	{
		return;
	}
	case 1:
	{
		statisticMonth();
		break;
	}
	case 2:
	{
		statisticUser();
		break;
	}
	default:
	{
		printf("输入操作错误！\n");
		Sleep(1000);
		return;
	}
	}
}

void selectmenu()
{
	int select = 0;
	char apwd[14];
	printf("请输入管理员权限密码：(其实是我的学号 *_* ):");
	scanf("%s",apwd);
	if (strcmp(apwd, password) == 0) {
		do {
			showmenu();
			printf("请输入您的选择(0-3):");
			scanf("%d", &select);
			system("cls");
			switch (select) {
			case 1:
				printf("【1】查询统计\n");
				statistic();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 2:
				printf("【2】激活卡\n");
				renew();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 3:
				printf("【3】计费标准\n");
				guanli();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 0://退出
				printf("******【0】退出******\n");
				system("pause>nul");//暂停
				system("cls");
				printf("欢迎下次使用！\n");

				return 0;
				
			default:
				printf("请输入正确的编号");
				break;
			}
		} while (select != 0);
	}
	else {
		printf("权限码错误！您无权访问！\n");
		return;
	}


}

