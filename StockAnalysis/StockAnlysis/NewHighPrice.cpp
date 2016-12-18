#include "StdAfx.h"
#include "NewHighPrice.h"
#include "Configuration.h"

NewHighPrice::NewHighPrice(const DataHandler& datahandler)
:Strategy(datahandler), m_cycle(20),newHighPrice(INVALID_VALUE)
{
}

NewHighPrice::~NewHighPrice(void)
{
	values.clear();
}

Strategy::ACTION NewHighPrice::Trigger(const Quot& quot)
{
	values.push_back(quot.Close);

	if (values.size() > m_cycle)
	{
		values.pop_front();
	}

	if (values.size() == m_cycle)
	{
		int nTmp = 0;
		int nMin = 99999999;

		foreach(values, it, deque<int>)
		{
			if (nTmp == 0)
			{
				nTmp = *it;
			}
			else
			{
				nTmp = max(*it, nTmp);
			}

			nMin = min(*it, nMin);
		}

		//
		if (nTmp == quot.Close)
		{
			newHighPrice = nTmp;

			values.clear();

			return Strategy::BUY;
		}
		else if (nMin == quot.Close)
		{
			newHighPrice = INVALID_VALUE;

			values.clear();

			return Strategy::SELL;
		}
	}

	return Strategy::NONE;
}

bool NewHighPrice::IsNewHighPrice()
{
	return (newHighPrice == INVALID_VALUE)?false:true;
}

void NewHighPrice::Initialize()
{
	m_cycle = g_Configuration.getMeanCycle();
	name_ = "NewHigh";
}

void NewHighPrice::ReInitialize(const DataHandler& datahandler)
{
	newHighPrice = INVALID_VALUE;

	m_cycle = datahandler.GetItem<int>("shortterm", g_Configuration.getMeanCycle());
}