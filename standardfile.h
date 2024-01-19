#pragma once
#ifndef standard_H
#define standard_H
#include "model.h"
int getstandardCount(const char* pPath);

int savestandard(const standard* pstandard, const char* ppath);

int updatastandard(const standard* pstandard, const char* ppath, int nIndex);

int readstandard(standard* pstandard, const char* ppath);

#endif 