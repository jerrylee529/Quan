#include "StdAfx.h"
#include "TrendMACrossStrategy.h"
#include "Configuration.h"

TrendMACrossStrategy::TrendMACrossStrategy(const DataHandler& datahandler)
: Strategy(datahandler), macd_(0)
{
	shortterm_ = datahandler.GetItem<int>("shortterm", g_Configuration.getMeanCycle());
	longterm_ = datahandler.GetItem<int>("longterm", g_Configuration.getAdjustCycle());
	threshold_ = datahandler.GetItem<int>("threshold", g_Configuration.getThreshold());
}

TrendMACrossStrategy::~TrendMACrossStrategy(void)
{
	if (maShortTerm)
	{
		delete maShortTerm;
	}

	if (maLongTerm)
	{
		delete maLongTerm;
	}

	if (macd_)
	{
		delete macd_;
	}
}

void TrendMACrossStrategy::Initialize()
{
	maShortTerm = new MALine(shortterm_);
	maLongTerm = new TrendMALine(longterm_, threshold_);
	macd_ = new MACD(12, 26, 9);
	name_ = "TrendCrossMA";
}

Strategy::ACTION TrendMACrossStrategy::Trigger(const Quot& quot)
{
	//
	macd_->Calculate(quot);

	// 计算短期均线 
	maShortTerm->Calculate(quot);

	// 计算长期均线
	maLongTerm->Calculate(quot);

	// 得到最新的短期均线价格
	int closeShortTerm =  maShortTerm->GetLatestClose();

	// 得到最新的长期均线价格
	int closeLongTerm = maLongTerm->GetLatestClose();

	// 无均线价格时退出
	if (closeShortTerm == INVALID_VALUE || closeLongTerm == INVALID_VALUE)
	{
		latestClose_ = quot.Close;

		return NONE;
	}

	// 计算最高价
	if (lastHigh_ != INVALID_VALUE)
	{
		lastHigh_ = max(lastHigh_, quot.High);
	}

	//
	int change = 0;

	if (lastHigh_ != 0 && lastHigh_ != INVALID_VALUE)
	{
		change = (lastHigh_ - quot.Close)*100/lastHigh_;
	}

	vector<MACDExtremePoint>& posExtPts = macd_->GetPosExtremePoints();

	vector<MACDExtremePoint>& negExtPts = macd_->GetNegExtremePoints();

	if (macd_->GetCount() <= 1 || posExtPts.size() < 2 || negExtPts.size() < 2)
	{
		return Strategy::NONE;
	}

	ACTION action = NONE;

	// 如果处于上升趋势且与长期均线交叉，则买入
	if (maShortTerm->GetLatestClose() > maShortTerm->GetLastClose()   // 当前短期价格高于前价格
		&& maShortTerm->GetLatestClose() > maLongTerm->GetLatestClose() // 当前短期价格高于当前长期价格
		&& maShortTerm->GetLastClose() <= maLongTerm->GetLastClose())// 前长期价格高于前短期价格
	{
		if (negExtPts[negExtPts.size()-1].macd > negExtPts[negExtPts.size()-2].macd && negExtPts[negExtPts.size()-1].close < negExtPts[negExtPts.size()-2].close)
		{
			action = BUY;
		}
	}
	else if ((/*maLongTerm->GetLatestClose() <= maLongTerm->GetLastClose() // 当前长期价格低于前价格
		&&*/ maShortTerm->GetLatestClose() < maLongTerm->GetLatestClose()  // 当前短期价格低于当前长期价格
		&& maShortTerm->GetLastClose() > maLongTerm->GetLastClose()
		&& maShortTerm->GetLatestClose() < maShortTerm->GetLastClose())
		/*|| change >= stop_loss_point_*/) // 当前长期价格高于前短期价格
	{
		if (posExtPts[posExtPts.size()-1].macd < posExtPts[posExtPts.size()-2].macd && posExtPts[posExtPts.size()-1].close > posExtPts[posExtPts.size()-2].close)
		{
			action = SELL;
		}
	}

	latestClose_ = quot.Close;

	return action;
}

void TrendMACrossStrategy::ReInitialize(const DataHandler& datahandler)
{
	if (maShortTerm)
	{
		delete maShortTerm;
	}

	if (maLongTerm)
	{
		delete maLongTerm;
	}

	if (macd_)
	{
		delete macd_;
	}

	shortterm_ = datahandler.GetItem<int>("shortterm", g_Configuration.getMeanCycle());
	longterm_ = datahandler.GetItem<int>("longterm", g_Configuration.getAdjustCycle());
	threshold_ = datahandler.GetItem<int>("threshold", g_Configuration.getThreshold());

	maShortTerm = new MALine(shortterm_);
	maLongTerm = new TrendMALine(longterm_, threshold_);

	macd_ = new MACD(12, 26, 9);
}

