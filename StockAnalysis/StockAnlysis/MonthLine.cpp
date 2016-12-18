#include "StdAfx.h"
#include "MonthLine.h"

MonthLine::MonthLine(vector<Quot>* monthlines)
: prev_month_(0), monthlines_(monthlines)
{
}

MonthLine::~MonthLine(void)
{
}

void MonthLine::Calculate(const Quot& quot)
{
	Date date;
	date.fromStringYYYYMMDD(quot.Datetime);

	// it's beginning of a week
	if (0 == prev_month_)  
	{
		quot_ = quot;
	}
	else if (date.month == prev_month_)
	{
		quot_.setDatetime(quot.Datetime);
		quot_.High = max(quot_.High, quot.High);
		quot_.Low = min(quot_.Low, quot.Low);
		quot_.Close = quot.Close;
		quot_.AdjClose = quot.AdjClose;
		quot_.Volume += quot.Volume;
	}
	else
	{
		if (quot_.Volume > 0) // filter the invalid information
		{
			monthlines_->push_back(quot_);
		}

		quot_ = quot;
	}

	prev_month_ = date.month;
}

