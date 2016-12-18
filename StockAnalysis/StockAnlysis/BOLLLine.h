#pragma once

#include "indicator.h"

//
struct tagBollinger
{
	double up;
	double down;
	double middle;
};

//
class BOLLLine :
	public Indicator
{
public:
	BOLLLine(int nCycle, int nTimes);
public:
	~BOLLLine(void);

	virtual void Calculate(const double value);

	string name();

private:
	int Cycle;
	int Times;
	deque<int> values;
	vector<tagBollinger> bollingerBand;
};
