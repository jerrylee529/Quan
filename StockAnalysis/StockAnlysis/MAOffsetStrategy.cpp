#include "StdAfx.h"
#include "MAOffsetStrategy.h"
#include "Configuration.h"

MAOffsetStrategy::MAOffsetStrategy(const DataHandler& datahandler)
: Strategy(datahandler), lastHigh_(INVALID_VALUE)
{
}

MAOffsetStrategy::~MAOffsetStrategy(void)
{
	if (maLine_)
	{
		delete maLine_;
	}
}

void MAOffsetStrategy::Initialize()
{
	maLine_ = new MALine(g_Configuration.getMeanCycle());

	name_ = "MAOffset";
}

Strategy::ACTION MAOffsetStrategy::Trigger(const Quot& quot)
{
	if (quot.Close <= 0)
	{
		return Strategy::NONE;
	}

	// 计算均线
	maLine_->Calculate(quot);

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


	ACTION action = NONE;

	int nStopLossRate = g_Configuration.getStopLoss();

	int offset = (quot.Close - maLine_->GetLatestClose()) * 100 / quot.Close;

	// 如果当前收盘价偏离均线（低于)
	if (offset < 0 && abs(offset) >= g_Configuration.getOffset())
	{
		lastHigh_ = quot.High;

		action = BUY;
	}
	else 
	{
		// 如果当前收盘价偏离均线（高于)
		if ((offset > 0 && abs(offset) >= g_Configuration.getOffset()) || change > nStopLossRate) 
		{
			lastHigh_ = INVALID_VALUE;

			action = SELL;
		}
	}


	return action;

}

void MAOffsetStrategy::ReInitialize(const DataHandler& datahandler)
{
	if (maLine_)
	{
		delete maLine_;
	}

	int shortterm_ = datahandler.GetItem<int>("shortterm", g_Configuration.getMeanCycle());

	maLine_ = new MALine(shortterm_);
}