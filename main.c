#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"menu.h"


int main()
{
		system("color  f9");

		printf("学号：0122210870531\t姓名：张俊鑫\t班级:计算机类y2205\n");


		Sleep(1000);//停顿以适应系统

		int select = 1;//用户选择变量
		char m[20] = { 0 };

		do {
			//主菜单的调用

			outputMenu();

			scanf("%s", m, 20);

			if (strlen(m) != 1) {
				system("cls");
				printf("输入有误，请重新输入：\n");
				continue;
			}
			select = m[0] - '0';
			system("cls");

			switch (select)
			{
			case 1://添加卡
				printf("******【1】添加卡:******\n\n");
				add();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 2://查询卡
				printf("******【2】查询卡:******\n\n");
				query();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 3://上机
				printf("******【3】上机:******\n\n");
				logon();
				system("pause>nul");//暂停
				system("cls");
				//
				break;
			case 4://下机
				printf("******【4】下机:******\n\n");
				settle();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 5://充值
				printf("******【5】充值:******\n\n");
				addMoney();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 6://退费
				printf("******【6】退费:******\n\n");
				refundMoney();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 7://注销卡
				printf("******【7】注销卡:******\n\n");
				annul();
				system("pause>nul");//暂停
				system("cls");

				break;
		    case 8://管理员操作
				printf("******【8】管理员操作:******\n\n");
				selectmenu();
				system("pause>nul");//暂停
				system("cls");
				break;
			case 0://退出
				printf("******【0】退出******\n");
				system("pause>nul");//暂停
				system("cls");
				printf("欢迎下次使用！\n");

				return 0;
				break;

			default:

				printf("输入有误，请重新输入：\n");
				continue;
				system("pause>nul");//暂停


				break;

			}




		} while (select != 0);

		system("pause");
		return 0;

}