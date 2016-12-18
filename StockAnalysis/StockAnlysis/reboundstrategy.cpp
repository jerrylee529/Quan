#include "StdAfx.h"
#include "reboundstrategy.h"
#include "Configuration.h"

ReboundStrategy::ReboundStrategy(const DataHandler& datahandler)
: Strategy(datahandler), lastClose_(INVALID_VALUE), latestLow_(INVALID_VALUE),macd_(0),
lastHigh_(INVALID_VALUE)
{

}

ReboundStrategy::~ReboundStrategy(void)
{
	if (maLine_)
	{
		delete maLine_;
	}

	if (maLongTerm)
	{
		delete maLongTerm;
	}
}

void ReboundStrategy::Initialize()
{
	maLine_ = new MALine(g_Configuration.getMeanCycle());
	maLongTerm = new MALine(g_Configuration.getAdjustCycle());

	macd_ = new MACD(12, 26, 9);

	name_ = "Rebound";
}

Strategy::ACTION ReboundStrategy::Trigger(const Quot& quot)
{
	// 计算macd
	macd_->Calculate(quot);

	// 计算均线
	maLine_->Calculate(quot);

	// 计算长期保护均线
	maLongTerm->Calculate(quot);

	// 得到最新的均线价格
	int maClose =  maLine_->GetLatestClose();

	// 无均线价格时退出
	if (maClose == INVALID_VALUE)
	{
		lastClose_ = quot.Close;

		return NONE;
	}

	vector<MACDExtremePoint>& posExtPts = macd_->GetPosExtremePoints();

	vector<MACDExtremePoint>& negExtPts = macd_->GetNegExtremePoints();

	if (macd_->GetCount() <= 1 || posExtPts.size() < 2 || negExtPts.size() < 2)
	{
		return Strategy::NONE;
	}

	ACTION action = NONE;

	if (macd_->getLastOne().macd < 0 && macd_->getLatestOne().macd < 0 && macd_->getLatestOne().macd > macd_->getLastOne().macd 
		&& macd_->getThirdOneFromEnd().macd < 0 && macd_->getLastOne().macd < macd_->getThirdOneFromEnd().macd 
		&& macd_->getLastOne().macd > negExtPts[negExtPts.size()-1].macd /*&& quot.getClose() < negExtPts[negExtPts.size()-1].close*/)
	{
		action = BUY;
	}
	else if (macd_->getLatestOne().macd > 0 && macd_->getLastOne().macd > 0 && macd_->getLatestOne().macd < macd_->getLastOne().macd
		&& macd_->getThirdOneFromEnd().macd > 0 && macd_->getLastOne().macd > macd_->getThirdOneFromEnd().macd
		&& posExtPts[posExtPts.size()-1].macd > macd_->getLastOne().macd /*&& quot.getClose() > posExtPts[posExtPts.size()-1].close*/)
	{
		action = SELL;
	}

	lastClose_ = quot.Close;

	return action;
}

void ReboundStrategy::ReInitialize(const DataHandler& datahandler)
{
	if (maLine_)
	{
		delete maLine_;
	}

	if (maLongTerm)
	{
		delete maLongTerm;
	}

	int shortterm_ = datahandler.GetItem<int>("shortterm", g_Configuration.getMeanCycle());
	int longterm_ = datahandler.GetItem<int>("longterm", g_Configuration.getAdjustCycle());
	int stop_loss_point_ = datahandler.GetItem<int>("stoplossrate", g_Configuration.getStopLoss());

	maLine_ = new MALine(shortterm_);
	maLongTerm = new MALine(longterm_);

	values.clear();
}
