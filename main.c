#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"menu.h"


int main()
{
		system("color  f9");

		printf("ѧ�ţ�0122210870531\t�������ſ���\t�༶:�������y2205\n");


		Sleep(1000);//ͣ������Ӧϵͳ

		int select = 1;//�û�ѡ�����
		char m[20] = { 0 };

		do {
			//���˵��ĵ���

			outputMenu();

			scanf("%s", m, 20);

			if (strlen(m) != 1) {
				system("cls");
				printf("�����������������룺\n");
				continue;
			}
			select = m[0] - '0';
			system("cls");

			switch (select)
			{
			case 1://��ӿ�
				printf("******��1����ӿ�:******\n\n");
				add();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 2://��ѯ��
				printf("******��2����ѯ��:******\n\n");
				query();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 3://�ϻ�
				printf("******��3���ϻ�:******\n\n");
				logon();
				system("pause>nul");//��ͣ
				system("cls");
				//
				break;
			case 4://�»�
				printf("******��4���»�:******\n\n");
				settle();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 5://��ֵ
				printf("******��5����ֵ:******\n\n");
				addMoney();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 6://�˷�
				printf("******��6���˷�:******\n\n");
				refundMoney();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 7://ע����
				printf("******��7��ע����:******\n\n");
				annul();
				system("pause>nul");//��ͣ
				system("cls");

				break;
		    case 8://����Ա����
				printf("******��8������Ա����:******\n\n");
				selectmenu();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 0://�˳�
				printf("******��0���˳�******\n");
				system("pause>nul");//��ͣ
				system("cls");
				printf("��ӭ�´�ʹ�ã�\n");

				return 0;
				break;

			default:

				printf("�����������������룺\n");
				continue;
				system("pause>nul");//��ͣ


				break;

			}




		} while (select != 0);

		system("pause");
		return 0;

}