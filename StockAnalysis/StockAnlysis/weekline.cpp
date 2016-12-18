#include "StdAfx.h"
#include "WeekLine.h"

WeekLine::WeekLine(vector<Quot>* weeklines)
:is_week_begining_(true), weeklines_(weeklines)
{
}

WeekLine::~WeekLine(void)
{
}

void WeekLine::Calculate(const Quot& quot)
{
	Date date;
	date.fromStringYYYYMMDD(quot.Datetime);

	// it's beginning of a week
	if (is_week_begining_)  
	{
		quot_ = quot;

		is_week_begining_ = false;
	}
	else if (date.isInSameWeek(prev_date_))
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
			//dest.insert(pair<string, Quot>(sti.getDate(), sti));
			weeklines_->push_back(quot_);
		}

		quot_ = quot;

		is_week_begining_ = false;
	}

	prev_date_ = date;
}

void WeekLine::Done()
{
	vector<Quot>::iterator it = find(weeklines_->begin(), weeklines_->end(), quot_);

	if (it == weeklines_->end())
	{
		weeklines_->push_back(quot_);
	}
}