#pragma once
#include "algorithm.h"
#include <string>


class TheLargestGain :
	public Algorithm
{
public:
	// 
	TheLargestGain(const char* szBeginDate, const char* szEndDate);
public:
	~TheLargestGain(void);

	virtual void Calculate(const Quot& quot);

	double GetValue();

private:
	double largestGain;
	string beginDate;
	string endDate;
	int open;
	int close;
};
