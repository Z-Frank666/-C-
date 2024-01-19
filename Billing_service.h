#pragma once
#include "model.h"

#ifndef BILLING_SERVICE_H
#define BILLING_SERVICE_H
int addBilling(const char* pName, Billing* pBilling);
Billing* queryBilling(const char* pName, int* pIndex);
//int saveBilling(const Billing* pbilling, const char* ppath);
int doStatisticUser(const char* aName, lpBillingNode* curHead);
int doStatisticMonth(const int nYear, const int nMonth, float* fTotalUse);

#endif