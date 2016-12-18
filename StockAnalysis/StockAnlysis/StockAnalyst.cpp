#include "StdAfx.h"
#include "StockAnalyst.h"
#include "Utilities.h"
#include <algorithm>

using namespace std;


StockAnalyst::StockAnalyst(void)
{
}

StockAnalyst::~StockAnalyst(void)
{
}

void StockAnalyst::calculateWeekLine(const deque<Quot>& src,  map<string, Quot>& dest)
{
	bool bWeekBegin = true;

	Quot sti;

	Date prevDate;

	for (deque<Quot>::const_iterator it = src.begin(); it != src.end(); it++)
    {
        Date date;
		date.fromStringYYYYMMDD(it->Datetime);

		// it's beginning of a week
		if (bWeekBegin)  
		{
			sti = *it;

			bWeekBegin = false;
		}
		else if (date.isInSameWeek(prevDate))
		{
			sti.setDatetime(it->Datetime);
			sti.High = max(sti.High, it->High);
			sti.Low = min(sti.Low, it->Low);
			sti.Close = it->Close;
			sti.AdjClose = it->AdjClose;
			sti.Volume += it->Volume;
		}
        else
		{
			if (sti.Volume > 0) // filter the invalid information
			{
				dest.insert(pair<string, Quot>(sti.getDate(), sti));
			}

			sti = *it;

			bWeekBegin = false;
		}

		prevDate = date;
    }

	map<string, Quot>::iterator iRet = dest.find(sti.getDate());

	if (iRet == dest.end())
	{
		dest.insert(pair<string, Quot>(sti.getDate(), sti));
	}
}

void StockAnalyst::calculateLinebyCycle(const deque<Quot>& src, 
										               int cycle,
													map<string, Quot>& dest)
{
	deque<Quot>::const_iterator it = src.begin();

	Quot sti = *it;

	int i = 1;
	for (it++; it != src.end(); it++)
	{
		i++;

		if (i > cycle)
		{
			dest.insert(pair<string, Quot>(sti.getDate(), sti));

			sti = *it;

			i = 1;

			continue;
		}

		sti.setDatetime(it->Datetime);
		sti.High = max(sti.High, it->High);
		sti.Low = min(sti.Low, it->Low);
		sti.Close = it->Close;
		sti.AdjClose = it->AdjClose;
		sti.Volume += it->Volume;
	}
}

void StockAnalyst::calculateMeanLine(const map<string, Quot>& src, const int cycle, map<string, int>& dest)
{
	Quot sti;

	deque<int> closes;

	int sumofclose = 0;

	if (src.size() >= cycle)
	{
		int i = 1;

		for (map<string, Quot>::const_iterator it = src.begin(); it != src.end(); it++)
		{
			sumofclose += it->second.Close;

			closes.push_back(it->second.Close);

			if (i > cycle)
			{
				sumofclose -= closes.at(0);

				closes.pop_front();

				dest.insert(pair<string, int>(it->first, sumofclose/cycle));				
			}
			else if (i == cycle)
			{
				dest.insert(pair<string, int>(it->first, sumofclose/cycle));

				i++;
			}
			else
			{
				i++;
			}
		}
	}
}

void StockAnalyst::calculatePossibility(const map<string, Quot>& weekLines, 
										const map<string, int>& meanLines,
										int& totalTimes, int& upTimes, int& downTimes)
{
	totalTimes = 0;
	upTimes = 0;
	downTimes = 0;

	for (map<string, int>::const_iterator it = meanLines.begin(); it != meanLines.end(); it++)
	{
		int mean = it->second;

		map<string, Quot>::const_iterator iRet = weekLines.find(it->first);

		if (iRet != weekLines.end())
		{
			bool bStart = true;

			int prevClose = 0;

			// cross the meanline
			if (iRet->second.Close > mean && iRet->second.Open < mean && iRet->second.Close > iRet->second.Open)
			{
                totalTimes++;

				prevClose = iRet->second.Close;
			}
			else
			{
				continue;
			}

			iRet++;

			// 
			if (iRet != weekLines.end() && iRet->second.Close > prevClose)
			{
				upTimes++;										
			}
			else
			{
				downTimes++;
			}
		}
	}
}