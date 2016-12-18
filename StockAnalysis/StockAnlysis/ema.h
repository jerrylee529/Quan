#pragma 
/********************************************************************
	created:	2015/05/23
	created:	23:5:2015   15:07
	filename: 	e:\Quan\StockAnlysis\StockAnlysis\ema.h
	file path:	e:\Quan\StockAnlysis\StockAnlysis
	file base:	ema
	file ext:	h
	author:		Jerry Li
	
	purpose:	º∆À„EMA
*********************************************************************/

#include <vector>

using namespace std;

class EMA
{
public:
	EMA(int cycle);

	~EMA(void);

public:
	void Calculate(const double value);

	int cycle() { return cycle_; }

	double GetLatestValue();

	vector<double>& items() { return items_; }

private:
	int cycle_;

	vector<double> items_;
};
