#pragma once
#include "algorithm.h"
#include <map>

using namespace std;

class WeekLine :
	public Algorithm
{
public:
	WeekLine(vector<Quot>* weeklines);
public:
	~WeekLine(void);

	void Calculate(const Quot& quot);

	void Done();

	vector<Quot>& GetWeekLines() { return *weeklines_; }
private:
	bool is_week_begining_;

	Quot quot_;

	Date prev_date_;

	vector<Quot>* weeklines_;
};
