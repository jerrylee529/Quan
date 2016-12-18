#include "StdAfx.h"
#include "GetMALine.h"


GetMALine::GetMALine(int nCycle)
:sumofclose_(0), sumofvol_(0), cycle_(nCycle)
{
}

GetMALine::~GetMALine(void)
{
	values.clear();
}

void GetMALine::calculate(const Quot& quot)
{
	values.push_back(quot);

	if (values.size() == cycle_)
	{
		foreach(values, it, deque<Quot>)
		{
			sumofclose_ += it->Close; 
			sumofvol_ += it->Volume;
		}
	}
	else if (values.size() > cycle_)
	{
		sumofclose_ -= values[0].Close;
		sumofvol_ -= values[0].Volume;
		sumofclose_ += values[values.size()-1].Close;
		sumofvol_ += values[values.size()-1].Volume;

		values.pop_front();
	}
	else
	{
		return;
	}

	int avg = sumofclose_/cycle_;

	MAs.insert(make_pair(quot.Datetime, avg));
}

