#pragma once
#include "algorithm.h"

class MonthLine :
	public Algorithm
{
public:
	MonthLine(vector<Quot>* monthlines);
	virtual ~MonthLine(void);

public:
	void Calculate(const Quot& quot);

private:
	Quot quot_;

	int prev_month_;

	vector<Quot>* monthlines_;
};
