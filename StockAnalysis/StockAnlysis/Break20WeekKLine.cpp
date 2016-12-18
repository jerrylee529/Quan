#include "StdAfx.h"
#include "Break20WeekKLine.h"
#include "StockAnalyst.h"
#include "Utilities.h"
#include "Configuration.h"
#include <fstream>


Break20WeekKLine::Break20WeekKLine(void):
totalTimes(0),upTimes(0),downTimes(0),morethan5percent(0)
{
}

Break20WeekKLine::~Break20WeekKLine(void)
{
}

void Break20WeekKLine::Calculate(const deque<Quot>& stockTradeInfos)
{
	StockAnalyst sa;

	// Calculate the week line
	map<string, Quot> weekLines;
	sa.calculateWeekLine(stockTradeInfos, weekLines);

	// Calculate the mean line
	map<string, int> meanLines;
	//sa.calculateMeanLine(weekLines, Singleton<Configuration>::getInstance().getMeanCycle(), meanLines);

	//

	for (map<string, int>::const_iterator it = meanLines.begin(); it != meanLines.end(); it++)
	{
		int mean = it->second;

		map<string, Quot>::const_iterator iRet = weekLines.find(it->first);

		if (iRet != weekLines.end())
		{
			bool bStart = true;

			int prevClose = 0;

			// cross the meanline
			if ((iRet->second.Close > mean) && (iRet->second.Open < mean) && (iRet->second.Close > iRet->second.Open))
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
			if (iRet != weekLines.end())
			{
				result res;

				if (iRet->second.Close > prevClose)
				{
					res.direct = UP;
					upTimes++;					

					if (100*(iRet->second.Close - prevClose)/prevClose > 5)
					{
						morethan5percent++;
					}
				}
				else
				{
					res.direct = DOWN;
					downTimes++;
				}

				res.percent = 100*abs(iRet->second.Close - iRet->second.Open)/prevClose;

				VEC_Result.push_back(res);
			}
		}
	}
}

void Break20WeekKLine::Export(const char* filename)
{
	ofstream fout;
	fout.open(filename);


	for (vector<result>::iterator it=VEC_Result.begin(); it != VEC_Result.end(); it++)
	{
		//fout<<it->date<<"\t"<<it->direct
	}
}

int Break20WeekKLine::GetUpPossibility()
{
	if (totalTimes > 0)
	{
		return 100*upTimes/totalTimes;
	}
	else
	{
		return 0;
	}
}

int Break20WeekKLine::GetDownPossibility()
{
	if (totalTimes > 0)
	{
		return 100*downTimes/totalTimes;
	}
	else
	{
		return 0;
	}
}

void Break20WeekKLine::update(Observable* observable, void* arg)
{
	/*
	int mean = it->second;

	map<string, Quot>::const_iterator iRet = weekLines.find(it->first);

	if (iRet != weekLines.end())
	{
		bool bStart = true;

		int prevClose = 0;

		// cross the meanline
		if ((iRet->second.Close > mean) && (iRet->second.Open < mean) && (iRet->second.Close > iRet->second.Open))
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
		if (iRet != weekLines.end())
		{
			result res;

			if (iRet->second.Close > prevClose)
			{
				res.direct = UP;
				upTimes++;					

				if (100*(iRet->second.Close - prevClose)/prevClose > 5)
				{
					morethan5percent++;
				}
			}
			else
			{
				res.direct = DOWN;
				downTimes++;
			}

			res.percent = 100*abs(iRet->second.Close - iRet->second.Open)/prevClose;

			VEC_Result.push_back(res);
		}
	}
	*/
}