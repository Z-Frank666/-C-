#pragma once
#ifndef CARD_FILE_H
#define CARD_FILE_H
int savecard(const Card* pcard, const char* ppath);//���������ݺ���
Card  prasecard(char* pbuf);//������������
int getcardcount(const char* ppath);//����ͳ�ƿ�������
int readcard(Card* pcard, const char* ppath);//������������
int updata(pcardnode pcard, const char* ppath);//�������¿�����-���ϻ��»�ʱ��ʹ��

#endif