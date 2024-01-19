#define _CRT_SECURE_NO_DEPRECATE
#include "tool.h"
#include<stdio.h>
#include<time.h>
#include"global.h"
#include<stdlib.h>

//将时间格式转化为年月日
void timetostring(time_t t, char* ptime)
{
	struct tm* pt;

	pt = localtime(&t);
	//声明time_t类型变量
   //获取系统日期和时间

	strftime(ptime, 20, "%Y-%m-%d %H:%M", pt);
	/*size_t strftime(char* str, size_t maxsize, const char* format, const struct tm* timeptr)
		根据 format 中定义的格式化规则，格式化结构 timeptr 表示的时间，并把它存储在 str 中。*/
}
//将字符串返回为时间类型
time_t  stringtotime(char* pTime)
{
	struct tm tml;
	time_t   time1;

	if (sscanf(pTime, "%d-%d-%d %d:%d", &tml.tm_year, &tml.tm_mon, &tml.tm_mday, &tml.tm_hour, &tml.tm_min) == FALSE) {
		printf("读取错误！");
	};
	/*int sscanf(const char* str, const char* format, ...)*/

	tml.tm_year -= 1900;  //年份为从1900年开始
	tml.tm_mon -= 1;      //月份为1^11
	tml.tm_sec = 0;
	tml.tm_isdst -= -1;
	//还原系统时间计数方法

	time1 = mktime(&tml);
	/*time_t mktime(struct tm* timeptr)*/
	//timeptr -- 这是指向表示日历时间的 time_t 值的指针，该日历时间被分解为以下各部分。下面是 timeptr 结构的细节：
	//	struct tm {
	//	int tm_sec;         /* 秒，范围从 0 到 59                */
	//	int tm_min;         /* 分，范围从 0 到 59                */
	//	int tm_hour;        /* 小时，范围从 0 到 23                */
	//	int tm_mday;        /* 一月中的第几天，范围从 1 到 31                    */
	//	int tm_mon;         /* 月份，范围从 0 到 11                */
	//	int tm_year;        /* 自 1900 起的年数                */
	//	int tm_wday;        /* 一周中的第几天，范围从 0 到 6                */
	//	int tm_yday;        /* 一年中的第几天，范围从 0 到 365                    */
	//	int tm_isdst;       /* 夏令时                        */
	//};

	return time1;
}

//获得time中的年份
int timeToYear(time_t t)
{
	struct tm* timeinfo;
	timeinfo = (struct tm*)malloc(sizeof(struct tm));
	timeinfo = localtime(&t);
	return timeinfo->tm_year + 1900;
}


//[得到time_t所表示的月份
int timeToMonth(time_t t)
{
	struct tm* timeinfo;
	timeinfo = (struct tm*)malloc(sizeof(struct tm));
	timeinfo = localtime(&t);
	return timeinfo->tm_mon + 1;
}