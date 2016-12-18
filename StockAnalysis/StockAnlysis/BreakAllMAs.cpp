#include "StdAfx.h"
#include "BreakAllMAs.h"

BreakAllMAs::BreakAllMAs(void)
{
}

BreakAllMAs::~BreakAllMAs(void)
{
	foreach(vtMAs, it, vector<MALine*>)
	{
		delete *it;
	}
}

void BreakAllMAs::SetMA(set<int>& mas)
{
	foreach(mas, it, set<int>)
	{
		MALine* ma = new MALine(*it);
     	vtMAs.push_back(ma);
	}
}

void BreakAllMAs::Calculate(const Quot& quot)
{
	set<int> cycles; 

	// 计算每条均线 
	foreach(vtMAs, it, vector<MALine*>)
	{
		(*it)->Calculate(quot);

		if ((*it)->getMAItems().size() > (*it)->cycle())
		{
			cycles.insert((*it)->cycle());
		}
	}

	// 如果不是所有均线都已存在则退出 
	if (cycles.size() < vtMAs.size())
	{
		return;
	}

	// 获得日期
	string strDate(quot.Datetime);

	// 计算是否跨越所有均线
	foreach(vtMAs, it, vector<MALine*>)
	{
		int price = (*it)->GetLatestClose();

		if ((quot.Close > price) && (quot.Open < price))
		{
			set<int>::iterator itSet = cycles.find((*it)->cycle());
			if (itSet != cycles.end())
			{
				cycles.erase(itSet);		
			}
		}
	}

	// 如果跨越所有均线，则将日期保留
	if (cycles.size() == 0)
	{
		vtDate.push_back(strDate);
	}
}
