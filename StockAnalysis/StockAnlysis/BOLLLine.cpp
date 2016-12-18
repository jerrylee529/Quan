#include "StdAfx.h"
#include "BOLLLine.h"
#include <math.h>

BOLLLine::BOLLLine(int nCycle, int nTimes)
:Cycle(nCycle), Times(nTimes)
{
}

BOLLLine::~BOLLLine(void)
{
}

double calcSTDDV(double& avg, deque<int>& va)
{
	// 求标准差 
	double sum = 0.0; 

	// 求和 	
	foreach(va, iter, deque<int>)
	{ 
		sum += *iter; 
	} 

	// 求平均值
	avg = sum / va.size();	

	sum = 0.0; 
	
	// 求标准差
	foreach(va, iter, deque<int>)
	{ 
		double tmp = *iter - avg; 
		
		sum += tmp * tmp; 
	} 

	double s = sqrt(sum / va.size()); 

	return s;
}

void BOLLLine::Calculate(const double value)
{
	values.push_back(value);

	if (values.size() > Cycle)
	{
		values.pop_front();
	}

	if (values.size() == Cycle)
	{
		double avg = 0;
		double dStddv = calcSTDDV(avg, values);

		tagBollinger boll;

		boll.up = avg + Times*dStddv;
		boll.down = avg - Times*dStddv;
		boll.middle = avg;

		bollingerBand.push_back(boll);
	}
	else
	{
		tagBollinger boll;
		boll.down = INVALID_VALUE;
		boll.up = INVALID_VALUE;
		boll.middle = INVALID_VALUE;
		bollingerBand.push_back(boll);
	}
}

string BOLLLine::name()
{
	return "BOLL";
}