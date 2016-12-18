#ifndef _STOCK_ANALYST_
#define _STOCK_ANALYST_

#include <deque>
#include <map>
#include "StockDataStructures.h"

using namespace std;

class StockAnalyst
{
public:
	StockAnalyst(void);
public:
	~StockAnalyst(void);

	// Calculate weekly K line
	void calculateWeekLine(const deque<Quot>& src, map<string, Quot>& dest);

	// Calculate weekly K line by cycle
	void calculateLinebyCycle(const deque<Quot>& src, int cycle, map<string, Quot>& dest);

	// Calculate mean line
	void calculateMeanLine(const map<string, Quot>& src, const int cycle, map<string, int>& dest);

	// Calculate possibility for 20 week line. return value is percentage
	void calculatePossibility(const map<string, Quot>& weekLines, const map<string, int>& meanLines,
		int& totalTimes, int& upTimes, int& downTimes);

	//                                                                                                                                                        
};


#endif
