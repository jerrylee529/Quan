#include "StdAfx.h"
#include "Strategy.h"
#include "CrossMAStrategy.h"
#include "TrendMACrossStrategy.h"
#include "TrendStrategy.h"
#include "VolumeCrossStrategy.h"
#include "MAStrategy.h"
#include "Configuration.h"
#include "macdstrategy.h"
#include "crossmacdstrategy.h"
#include "reboundstrategy.h"
#include "MAOffsetStrategy.h"
#include <assert.h>

//
Strategy::Strategy(const DataHandler& datahandler)
:event_listener_(0),stop_loss_point_(10),name_(""),
latestClose_(INVALID_VALUE)
{
	stop_loss_point_ = datahandler.GetItem<int>("stop_loss_rate", g_Configuration.getStopLoss());
}

Strategy::~Strategy(void)
{
}

Strategy* Strategy::Create(const char* strategyname, const DataHandler& datahandler)
{
	string strName(strategyname);

	Strategy* strategy = 0;

	if (strName == "Trend")
	{
		strategy = new TrendStrategy(datahandler);
	}
	else if (strName == "MA")
	{
		strategy = new MAStrategy(datahandler);
	}
	else if (strName == "CrossMA")
	{
		strategy = new CrossMAStrategy(datahandler);
	}
	else if (strName == "VolumeCross")
	{
		strategy = new VolumeCrossStrategy(datahandler);
	}
	else if (strName == "TrendCrossMA")
	{
		strategy = new TrendMACrossStrategy(datahandler);
	}
	else if (strName == "CrossMACD")
	{
		strategy = new CrossMACDStrategy(datahandler);
	}
	else if (strName == "NewHigh")
	{
		strategy = new NewHighPrice(datahandler);
	}
	else if (strName == "MACD")
	{
		strategy = new MACDStrategy(datahandler);
	}
	else if (strName == "Rebound")
	{
		strategy = new ReboundStrategy(datahandler);
	}
	else if (strName == "MAOffset")
	{
		strategy = new MAOffsetStrategy(datahandler);
	}
	else
	{
		return 0;
	}

	strategy->Initialize();

	return strategy;
}

void Strategy::DoOnSell(const Quot& quot)
{
	if (event_listener_)
	{
		event_listener_->DoOnSell(quot);
	}
}

void Strategy::DoOnBuy(const Quot& quot)
{
	if (event_listener_)
	{
		event_listener_->DoOnBuy(quot);
	}
}


