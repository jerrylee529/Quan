#pragma once
#include "algorithm.h"
#include <map>

using namespace std;

class GetWeekLine :
	public Algorithm
{
public:
	GetWeekLine(void);
public:
	~GetWeekLine(void);

	void calculate(const Quot& quot);

	void done();

private:
	bool bWeekBegin;

	Quot sti;

	Date prevDate;

	map<string, Quot> dest;
};
