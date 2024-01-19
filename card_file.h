#pragma once
#ifndef CARD_FILE_H
#define CARD_FILE_H
int savecard(const Card* pcard, const char* ppath);//声明存数据函数
Card  prasecard(char* pbuf);//声明解析函数
int getcardcount(const char* ppath);//声明统计卡数函数
int readcard(Card* pcard, const char* ppath);//声明读卡函数
int updata(pcardnode pcard, const char* ppath);//声明更新卡函数-在上机下机时会使用

#endif