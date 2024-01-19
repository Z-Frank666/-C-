#pragma once
#include<time.h>
#ifndef TOOL_H
#define TOOL_H

void timetostring(time_t t, char* ptime);

time_t stringtotime(char* ptime);

int timeToYear(time_t t);

int timeToMonth(time_t t);
#endif