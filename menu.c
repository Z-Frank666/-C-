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

void outputMenu()//���˵�
{

	printf("��ӭ����Ʒѹ���ϵͳ\n");
	printf("----------�˵�----------\n");
	printf("     ��1����ӿ� \n");
	Sleep(30);
	printf("     ��2����ѯ��\n");
	Sleep(30);
	printf("     ��3���ϻ�\n");
	Sleep(30);
	printf("     ��4���»�\n");
	Sleep(30);
	printf("     ��5����ֵ\n");
	Sleep(30);
	printf("     ��6���˷�\n");
	Sleep(30);
	printf("     ��7��ע����\n");
	Sleep(30);
	printf("     ��8������Ա����\n");
	Sleep(30);
	printf("     ��0���˳�\n\n");
	printf("��ѡ��˵�����(0-8):\n");

}

//��ӿ�
void add()
{
	struct Card card;

	char aName[50] = { '\0' };
	char apwd[20] = { '\0' };

	//��ʼ��ʱ��
	struct  tm* startTime;//����ʱ��
	struct tm* endTime;//��ֹʱ��
	//����ʱ�䣬��ֹʱ������ʹ��ʱ�䶼Ĭ��Ϊ��ǰ��ϵͳʱ��
	card.tStart = card.tEnd = card.tLast = time(NULL);
	startTime = localtime(&card.tStart);
	endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	card.tEnd = mktime(endTime);//��ʱ��ת��Ϊtime_t����

	printf("�����뿨��<����Ϊ0~18>:");

	scanf("%s", aName, 50);

	if (nameRecheck(aName) == TRUE)
	{
		printf("�˺���ע�ᣬ������ע��");
		return;
	}
	//�˺ŷ���ṹ��
	strcpy(card.aName, aName);

	printf("\n����������<����Ϊ0~8>��");

	scanf("%s", apwd, 20);
	strcpy(card.apwd, apwd);

	//����Ƿ񳬳�
	if (strlen(aName) > 18 || strlen(apwd) > 8) {
		printf("�˺Ż����������ʽ����\n");

		return;
	}
	
	printf("\n�����뿪�����:");

	scanf("%f", &card.balance);

	card.totaluse = 0;

	card.usecount = 0;

	card.status = 0;

	card.del = 0;

	//�����״̬
	printf("\n");

	printf("----------��ӿ���Ϣ����-------\n");

	printf("����\t����\t��״̬\t���\t\n");

	printf("%s\t%s\t%d\t%0.1f\t\n", card.aName, card.apwd, card.status, card.balance);

	//���¿������ļ�
	addcard(card);


}

void query()//��ѯ���ĳ�ʼ����
{
	int select = 0;

	struct Card* qcard = NULL;

	char aName[18] = { 0 };
	char aTime[20] = { 0 };
	int n = 0;//�ж��Ƿ�鵽

	printf("��ѡ����Ĳ�ѯ��ʽ��\n");
	printf("��1��    ģ����ѯ\n");
	printf("��2��    ��ȷ��ѯ\n");


	printf("���ѡ��");

	scanf("%d", &select);

	printf("\n");

	switch (select)
	{
	case 1:
		printf("�������ѯ���ţ�");
		scanf("%s", aName);
		qcard = querycard_1(aName, &n);//��ʱqcard��ֵӦΪ���鿨
		break;
	case 2:
		printf("�������ѯ���ţ�");
		scanf("%s", aName);
		qcard = querycard_2(aName, &n);//��ʱqcard��ֵӦΪ���鿨
		break;
	}

	if (qcard == NULL || n == 0)
	{
		printf("û�в�ѯ��������Ϣ");
	}

	int i = 0;//����ģ����ѯ����

	if(n)//��ѯ�ɹ�ʱ��nֵӦΪ����(��ѡ��ȷ��ѯ��n��Ϊ1)
	{
		//�����ѯ���Ŀ�
		printf("����\t��״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\t\n");

		while (i < n) {

			timetostring(qcard[i].tLast, aTime);

			printf("%s\t%d\t%0.1f\t%0.1f\t\t%d\t\t%s\n", qcard[i].aName, qcard[i].status, qcard[i].balance, qcard[i].totaluse, qcard[i].usecount, aTime);

			i++;
		}
	}
}


//�ϻ�
void logon()
{
	char aName[18] = { 0 };
	char apwd[8] = { 0 };
	Card* pcard = NULL;//����һ������Ϣ
	char lasttime[TIME] = { 0 };//����һ���ַ��������ϻ�ʱ��
	LogonInfo* pInfo = NULL;//�����ϻ���Ϣ
	int nResult = 0;//�ж��Ƿ����ϻ��Ĳ���

	memset(aName, 0, sizeof(aName));
	memset(apwd, 0, sizeof(apwd));
	memset(lasttime, 0, sizeof(lasttime));

	//��ʾ���ҽ����û����뿨�ź�����
	printf("�������ϻ����ţ�����Ϊ1-17����");
	scanf("%s", aName);

	//�ж�����Ŀ����Ƿ����Ҫ��
	if (strlen(aName) >= 18)
	{
		printf("���뿨�ų��ȳ������ֵ��\n");

	}

	//��ʾ�������ϻ����룻
	printf("�������ϻ����루����Ϊ1-7��:");
	scanf("%s", apwd);


	//�ж�����Ŀ��������Ƿ����Ҫ��
	if (strlen(apwd) >= 8)
	{
		printf("���뿨�ų��ȳ������ֵ��\n");
	}

	//��ʼ�ϻ�����ȡ�ϻ���Ϣ
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));

	//�����ϻ��Ľ������ʾ�����ͬ��Ϣ
	nResult = doLogon(aName, apwd, pInfo);//�ж��ϻ����
	switch (nResult)
	{
	case 0:
		printf("------------�ϻ�ʧ�ܣ�------------\n"); break;
	case 1:
		printf("-----******-----�ϻ���Ϣ����-----******-----\n");
		printf("����              ���              �ϻ�ʱ��\n");		//18 18

		timetostring(pInfo->tLogon, lasttime);

		//�����Ϣ
		printf("%s", pInfo->aCardName);
		printf("%20.2f", pInfo->fBalance);
		printf("%30s\n", lasttime);
		printf("�ϻ��ɹ���\n");
		break;
	case 2:
		printf("------------�ÿ�����ʹ�ã�------------\n"); break;
	case 3:
		printf("------------���㣡------------\n"); break;
	}
	free(pInfo);
}

//�»�ʵ��
void settle()
{
	Card* scard = NULL;
	SettleInfo* pInfo = NULL;
	char tlogout[20] = { '\0' };
	char scardname[20] = { '\0' };
	char spwd[10] = { '\0' };
	printf("�������»����ţ�");
	scanf("%s",scardname);
	printf("\n");
	printf("�������»������룺");
	scanf("%s",spwd);

	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	//�����»�״̬
	scard = doSettle(scardname,spwd,pInfo);

	if (scard == NULL)
		printf("\n�»�ʧ��!\n");
	else
	{
		printf("���ţ�%s\t", scard->aName);
		printf("��%.2f\t", scard->balance);
		timetostring(pInfo->tEnd, tlogout);
		printf("�»�ʱ�䣺%s\t", tlogout);
		printf("\n�»��ɹ�!\n");
	}

}

//��ֵ
void addMoney()
{
	Card* acard = NULL;
	double money;
	char fcardname[18] = { '\0' };
	char fpassword[8] = { '\0' };
	printf("�����뿨��<����Ϊ1-18>��");
	scanf("%s", &fcardname);           //���տ��ź�����
	printf("����������<����Ϊ1-8>��");
	scanf("%s", &fpassword);
	printf("�������ֵ���<RMB>��");	//��ѭ��������������ַ�(��)��������ѭ�������⣬��main�������÷���ͬ
	while (scanf("%lf", &money) != 1)
	{
		printf("������ĳ�ֵ�������(������������С��)\n");
		printf("����������:\n");
		while (getchar() != '\n');
	}

	acard = doAddMoney(fcardname, fpassword, money);

	if (acard == NULL)
		printf("\n��ֵʧ��\n");
	else
	{
		printf("\n��ֵ�ɹ�\n");
		printf("���ţ�%s\n", acard->aName);
		printf("��%.2f\n", acard->balance);
	}
}

//�˷�
void refundMoney() {
	Card* acard = NULL;
	double money;
	char fcardname[18] = { '\0' };
	char fpassword[8] = { '\0' };
	printf("�����뿨��<����Ϊ1-18>��");
	scanf("%s", &fcardname);
	printf("����������<����Ϊ1-8>��");
	scanf("%s", &fpassword);
	printf("�������˷ѽ��<RMB>��");
	//��ѭ��������������ַ�(��)��������ѭ�������⣬��main�������÷���ͬ
	while (scanf("%lf", &money) != 1) {
		printf("��������˷ѽ������(������������С��)\n");
		printf("����������:\n");
		while (getchar() != '\n');
	}

	acard = doRefunMoney(fcardname, fpassword, money);
	if (acard == NULL)
		printf("\n�˷�ʧ��\n");
	else
	{
		printf("\n�˷ѳɹ�\n");
		printf("���ţ�%s\n", acard->aName);
		printf("��%.2f\n", acard->balance);
	}

}

//ע����
void annul()
{
	Card* acard = NULL;
	int n = 0;
	char fcardname[18] = { '\0' };
	char fpassword[8] = { '\0' };
	printf("�����뿨��<����Ϊ1-18>��");
	scanf("%s", &fcardname);
	printf("����������<����Ϊ1-8>��");
	scanf("%s", &fpassword);

	acard = doAnnul(fcardname, fpassword);
	if (acard == NULL)
		printf("\nע��ʧ��\n");
	else
		printf("\nע���ɹ�\n");


}

void showmenu()
{
	printf("----------�˵�----------\n");
	printf("     ��1����ѯͳ�� \n\n");
	Sleep(30);
	printf("     ��2�����\n\n");
	Sleep(30);
	printf("     ��3���Ʒѱ�׼\n\n");
	Sleep(30);
	printf("     ��0���˳�\n\n");

}

//���¼��
void renew()
{
	Card* acard = NULL;
	char tend[20] = { '\0' };
	int n = 0;
	char fcardname[18] = { '\0' };
	char fpassword[8] = { '\0' };
	printf("�����뿨��<����Ϊ1-18>��");
	scanf("%s", &fcardname);
	printf("����������<����Ϊ1-8>��");
	scanf("%s", &fpassword);
	acard = doRenew(fcardname, fpassword);
	if (acard == NULL)
		printf("\n���¼���ʧ��\n");
	else
	{
		printf("\n���¼���ɹ�\n");
		printf("���ţ�%s\n", acard->aName);
		timetostring(acard->tEnd, tend);
		/*printf("����ֹʱ���Ѹ���Ϊ��%s\n", tend);*/
	}
}

//��ʾ���мƷѱ�׼��Ϣ
void allstandard()
{
	printf("���еĿ���Ϣ����\n");
	showallstandard();
}


//�Ʒѹ���ϵͳ��׼ѡ�����ʾ
void showguanli()
{
	printf("��ӭ����Ʒѱ�׼����\n\n");
	printf("1.������׼\n");
	printf("2.��ѯ��׼\n");
	printf("3.�޸ı�׼\n");
	/*printf("4.ɾ����׼\n");*/
	printf("0.�˳�\n\n");
}

//�ԼƷѱ�׼���в���

void guanli()
{
	char* name;
	int choose = 0;
	standard* p1standard;
	standard* p2standard;
	int nPosition = 0;

	p1standard = (standard*)malloc(sizeof(standard));
	showguanli();

	printf("���������ѡ��(0--3):");

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
		printf("��ӭ�´�ʹ��");
		return;
	default:
		printf("��������ȷ�ı��");
		break;
	}
}

//ͳ��
void statistic()
{
	int nSelection = -1;
	printf("��1��.���·�ͳ���û�ʹ����Ӫҵ��\n");
	printf("��2��.���û�����ͳ�Ƹ��û�ʹ�ý��\n");
	printf("��0��.����\n");
	printf("��ѡ����Ҫ���е�ͳ�Ʋ�����");

	//��ȡ�û�����
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
		printf("�����������\n");
		Sleep(1000);
		return;
	}
	}
}

void selectmenu()
{
	int select = 0;
	char apwd[14];
	printf("���������ԱȨ�����룺(��ʵ���ҵ�ѧ�� *_* ):");
	scanf("%s",apwd);
	if (strcmp(apwd, password) == 0) {
		do {
			showmenu();
			printf("����������ѡ��(0-3):");
			scanf("%d", &select);
			system("cls");
			switch (select) {
			case 1:
				printf("��1����ѯͳ��\n");
				statistic();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 2:
				printf("��2�����\n");
				renew();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 3:
				printf("��3���Ʒѱ�׼\n");
				guanli();
				system("pause>nul");//��ͣ
				system("cls");
				break;
			case 0://�˳�
				printf("******��0���˳�******\n");
				system("pause>nul");//��ͣ
				system("cls");
				printf("��ӭ�´�ʹ�ã�\n");

				return 0;
				
			default:
				printf("��������ȷ�ı��");
				break;
			}
		} while (select != 0);
	}
	else {
		printf("Ȩ�����������Ȩ���ʣ�\n");
		return;
	}


}

