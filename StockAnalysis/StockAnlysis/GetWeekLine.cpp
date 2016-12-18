#include "StdAfx.h"
#include "GetWeekLine.h"

GetWeekLine::GetWeekLine(void)
:bWeekBegin(true)
{
}

GetWeekLine::~GetWeekLine(void)
{
}

void GetWeekLine::calculate(const Quot& quot)
{
	Date date;
	date.fromStringYYYYMMDD(quot.Datetime);

	// it's beginning of a week
	if (bWeekBegin)  
	{
		sti = quot;

		bWeekBegin = false;
	}
	else if (date.isInSameWeek(prevDate))
	{
		sti.setDatetime(quot.Datetime);
		sti.High = max(sti.High, quot.High);
		sti.Low = min(sti.Low, quot.Low);
		sti.Close = quot.Close;
		sti.AdjClose = quot.AdjClose;
		sti.Volume += quot.Volume;
	}
	else
	{
		if (sti.Volume > 0) // filter the invalid information
		{
			dest.insert(pair<string, Quot>(sti.getDate(), sti));
		}

		sti = quot;

		bWeekBegin = false;
	}

	prevDate = date;
}

void GetWeekLine::done()
{
	map<string, Quot>::iterator iRet = dest.find(sti.getDate());

	if (iRet == dest.end())
	{
		dest.insert(pair<string, Quot>(sti.getDate(), sti));
	}
}