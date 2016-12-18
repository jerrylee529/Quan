#include "StdAfx.h"
#include "maline.h"
#include <assert.h>


MALine::MALine(int nCycle)
:sumofclose_(0), sumofvol_(0), cycle_(nCycle),
maTrendType_(MA_UNKNOWN)
{
	assert(nCycle > 0);
}

MALine::~MALine(void)
{
	values.clear();

	vtMAItem.clear();
}

void MALine::Calculate(const Quot& quot)
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
		vtMAItem.push_back(INVALID_VALUE);

		return;
	}

	int avg = sumofclose_/cycle_;

	vtMAItem.push_back(avg);
}

int MALine::GetLatestClose()
{
	if (!vtMAItem.empty())
	{
		return vtMAItem[vtMAItem.size()-1];
	}
	else
	{
		return INVALID_VALUE;
	}
}

int MALine::GetLastClose()
{
	if (vtMAItem.size() > 2)
	{
		return vtMAItem[vtMAItem.size()-2];
	}
	else
	{
		return INVALID_VALUE;
	}
}

int MALine::GetCurrTrendType()
{
	if (vtMAItem.size() > 2)
	{
		int latestPrice = vtMAItem[vtMAItem.size()-1];

		int lastPrice = vtMAItem[vtMAItem.size()-2];

		return (latestPrice>lastPrice)?MA_UP:MA_DOWN;
	}
	else
	{
		return MA_UNKNOWN;
	}
}

vector<int>& MALine::getMAItems()
{
	return vtMAItem;
}