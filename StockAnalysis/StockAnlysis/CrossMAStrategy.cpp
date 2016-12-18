#include "StdAfx.h"
#include "CrossMAStrategy.h"
#include "Utilities.h"
#include "Configuration.h"


const int MACD_NONE = -1;
const int FIRST_HIGH = 0;
const int FIRST_LOW = 1;
const int SECOND_HIGH = 2;
const int SELL_POINT = 3;

CrossMAStrategy::CrossMAStrategy(const DataHandler& datahandler)
: Strategy(datahandler), maShortTerm(0), maLongTerm(0), maSafeTerm(0), 
lastVolume_(INVALID_VALUE),
volIncrease_(2), lastHigh_(INVALID_VALUE), macd_(0)
{
	shortterm_ = datahandler.GetItem<int>("shortterm", g_Configuration.getMeanCycle());
	longterm_ = datahandler.GetItem<int>("longterm", g_Configuration.getAdjustCycle());
	safeterm_ = datahandler.GetItem<int>("safeterm", g_Configuration.safeMA_cycle());
	stop_loss_point_ = datahandler.GetItem<int>("stoplossrate", g_Configuration.getStopLoss());

	lastMACD_.macd = INVALID_VALUE;
}

CrossMAStrategy::~CrossMAStrategy(void)
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

void CrossMAStrategy::Initialize()
{
	maShortTerm = new MALine(shortterm_);
	maLongTerm = new MALine(longterm_);
	maSafeTerm = new MALine(safeterm_);
	macd_ = new MACD(12, 26, 9);
	macdstatus_ = MACD_NONE;
	name_ = "CrossMA";
}

void CrossMAStrategy::ReInitialize(const DataHandler& datahandler)
{
	if (maShortTerm)
	{
		delete maShortTerm;
	}

	if (maLongTerm)
	{
		delete maLongTerm;
	}

	if (maSafeTerm)
	{
		delete maSafeTerm;
	}

	shortterm_ = datahandler.GetItem<int>("shortterm", g_Configuration.getMeanCycle());
	longterm_ = datahandler.GetItem<int>("longterm", g_Configuration.getAdjustCycle());
	safeterm_ = datahandler.GetItem<int>("safeterm", g_Configuration.safeMA_cycle());
	stop_loss_point_ = datahandler.GetItem<int>("stoplossrate", g_Configuration.getStopLoss());

	macdstatus_ = MACD_NONE;

	maShortTerm = new MALine(shortterm_);
	maLongTerm = new MALine(longterm_);
	maSafeTerm = new MALine(safeterm_);
}

Strategy::ACTION CrossMAStrategy::Trigger(const Quot& quot)
{
	macd_->Calculate(quot);

	// 计算短期均线
	maShortTerm->Calculate(quot);

	// 计算长期均线
	maLongTerm->Calculate(quot);

	//
	//maSafeTerm->Calculate(quot);

	vector<MACDExtremePoint>& posExtPts = macd_->GetPosExtremePoints();

	vector<MACDExtremePoint>& negExtPts = macd_->GetNegExtremePoints();

	if (macd_->GetCount() <= 1 || posExtPts.size() < 2 || negExtPts.size() < 2)
	{
		return Strategy::NONE;
	}

	// 得到最新的均线价格
	int closeShortTerm =  maShortTerm->GetLatestClose();

	int closeLongTerm = maLongTerm->GetLatestClose();

	// 无均线价格时退出
	if (closeShortTerm == INVALID_VALUE || closeLongTerm == INVALID_VALUE)
	{
		lastVolume_ = quot.getVolume();

		latestClose_ = quot.Close;

		return NONE;
	}

	// 计算最高价
	if (lastHigh_ != INVALID_VALUE)
	{
		lastHigh_ = max(lastHigh_, quot.High);
	}

	//
	float change = 0;
	if (lastHigh_ != 0 && lastHigh_ != INVALID_VALUE)
	{
		change = (lastHigh_ - quot.Close)*100/lastHigh_;
	}


	//
	ACTION action = NONE;

	// 如果处于上升趋势且与长期均线交叉，则买入
	if (maShortTerm->GetLatestClose() > maShortTerm->GetLastClose()   // 当前短期价格高于前价格
		&& maShortTerm->GetLatestClose() > maLongTerm->GetLatestClose() // 当前短期价格高于当前长期价格
		&& maShortTerm->GetLastClose() <= maLongTerm->GetLastClose() // 前长期价格高于前短期价格
		) // 长期均线价格大于前价格
	{
		lastHigh_ = quot.High;

		lastMACD_ = macd_->getLatestOne();

		action = BUY;
	}
	else if ((maShortTerm->GetLatestClose() < maShortTerm->GetLastClose() // 当前短期价格低于前价格
		&& maShortTerm->GetLatestClose() < maLongTerm->GetLatestClose()  // 当前短期价格低于当前长期价格
		&& maShortTerm->GetLastClose() >= maLongTerm->GetLastClose()) // 短期均线前价格大于长期均线前价格
		) // 当前长期价格高于前短期价格
	{
		lastHigh_ = INVALID_VALUE;

		action = SELL;
	}
	else
	{
	}

	lastVolume_ = quot.getVolume();

	latestClose_ = quot.Close;

	return action;
}

bool CrossMAStrategy::isUpTrend()
{
	if (trendArray_.size() > 2)
	{
		int& nFirst = trendArray_.front();

		size_t i = 0;

		foreach(trendArray_, it, deque<int>)
		{
			if (*it < nFirst)
			{
				break;
			}

			nFirst = *it;

			i++;
		}

		return (i == trendArray_.size())?true:false;
	}
	else
	{
		return false;
	}
}


