#include "StdAfx.h"
#include "crossmacdstrategy.h"
#include "Configuration.h"

const string SHORT_TERM = "short_term";
const string LONG_TERM = "long_term";
const string DIF_TERM = "dif_term";

CrossMACDStrategy::CrossMACDStrategy(const DataHandler& datahandler)
:Strategy(datahandler), macd_(0), last_high_(INVALID_VALUE), search_buy_point(false), maline_(0)
{
	memset(&last_quot_, 0, sizeof(last_quot_));
	last_quot_.Close = INVALID_VALUE;
	stop_loss_point_ = datahandler.GetItem<int>("stoplossrate", g_Configuration.getStopLoss());
}

CrossMACDStrategy::~CrossMACDStrategy(void)
{

}

void CrossMACDStrategy::Initialize()
{
	macd_.reset(new MACD(12, 26, 9));

	maline_.reset(new MALine(g_Configuration.getMeanCycle()));

	name_ = "CrossMACD";
}

Strategy::ACTION CrossMACDStrategy::Trigger(const Quot& quot)
{
	// 计算macd
	macd_->Calculate(quot);

	if (last_quot_.Close == INVALID_VALUE)
	{
		last_quot_ = quot;

		return Strategy::NONE;
	}

	// 计算均线
	maline_->Calculate(quot);

	//
	vector<MACDItem>& items = macd_->items();

	if (items.size() < 3)
	{
		last_quot_ = quot;

		return Strategy::NONE;
	}

	MACDItem& last_one = macd_->getLastOne();

	MACDItem& latest_one = macd_->getLatestOne();

	// 计算最高价
	if (last_high_ != INVALID_VALUE)
	{
		last_high_ = max(last_high_, quot.High);
	}

	//
	float change = 0;
	if (last_high_ != 0 && last_high_ != INVALID_VALUE)
	{
		change = (last_high_ - quot.Close)*100/last_high_;
	}


	//
	ACTION action = Strategy::NONE;

	// 如果dif上穿dea轴
	if (last_one.dif < latest_one.dif && last_one.dif < last_one.dea 
		&& latest_one.dif > latest_one.dea && latest_one.dif < 0 
		&& last_one.dea < 0 && latest_one.dif < 0 && latest_one.dea < 0 && latest_one.macd > -2)   
	{
		action = BUY;
		/*
		if (search_buy_point) 
		{
			action = BUY;

			search_buy_point = false;
		}
		else
		{
			last_macditem_ = latest_one;

			search_buy_point = true;
		}
		*/
	}
	else if (last_one.dif > latest_one.dif && last_one.dif > last_one.dea 
		&& latest_one.dif < latest_one.dea && last_one.dif > 0
		&& last_one.dea > 0 && latest_one.dif > 0 && latest_one.dea > 0) // 当前dif下穿dea
	{
		action = SELL;
	}
	else
	{
	}

	last_quot_ = quot;

	return action;
}

void CrossMACDStrategy::ReInitialize(const DataHandler& datahandler)
{
	int short_term = datahandler.GetItem<int>(SHORT_TERM, 12);
	int long_term = datahandler.GetItem<int>(LONG_TERM, 26);
	int dif_term = datahandler.GetItem<int>(DIF_TERM, 9);

	last_high_ = INVALID_VALUE;

	stop_loss_point_ = datahandler.GetItem<int>("stoplossrate", g_Configuration.getStopLoss());

	macd_.reset(new MACD(short_term, long_term, dif_term));

	maline_.reset(new MALine(g_Configuration.getMeanCycle()));
}