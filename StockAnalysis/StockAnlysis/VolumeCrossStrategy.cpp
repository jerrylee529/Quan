#include "StdAfx.h"
#include "VolumeCrossStrategy.h"
#include "Configuration.h"

VolumeCrossStrategy::VolumeCrossStrategy(const DataHandler& datahandler)
: Strategy(datahandler)
{
	shortterm_ = datahandler.GetItem<int>("shortterm", g_Configuration.getMeanCycle());
	longterm_ = datahandler.GetItem<int>("longterm", g_Configuration.getAdjustCycle());
}

VolumeCrossStrategy::~VolumeCrossStrategy(void)
{
	if (ma_shortterm_)
	{
		delete ma_shortterm_;
	}

	if (ma_longterm_)
	{
		delete ma_longterm_;
	}
}

void VolumeCrossStrategy::Initialize()
{
	ma_shortterm_ = new MA(shortterm_);
	ma_longterm_ = new MA(longterm_);
	name_ = "VolumeCross";
}

Strategy::ACTION VolumeCrossStrategy::Trigger(const Quot& quot)
{
	ma_shortterm_->Calculate(quot.Volume);

	ma_longterm_->Calculate(quot.Volume);

	// 得到最新的均线价格
	double short_latest = ma_shortterm_->GetLatestValue();

	//
	double long_latest = ma_longterm_->GetLatestValue();

	// 无均线值时退出
	if (short_latest == INVALID_VALUE || long_latest == INVALID_VALUE)
	{
		latestClose_ = quot.Close;

		return NONE;
	}


	Strategy::ACTION action = NONE;

	// 如果处于上升趋势且与长期均线交叉，则买入
	if (ma_shortterm_->GetLatestValue() > ma_shortterm_->GetLastValue()   // 当前短期价格高于前价格
		&& ma_shortterm_->GetLatestValue() > ma_longterm_->GetLatestValue() // 当前短期价格高于当前长期价格
		&& ma_shortterm_->GetLastValue() < ma_longterm_->GetLatestValue())// 前长期价格高于前短期价格
	{
		action = BUY;

		//LOG("买入, 日期: %s, 价格: %d, 当前金额: %.2f, 仓位: %.2f", quot.Date, quot.Close, amount_, position_);
	}
	else if (ma_longterm_->GetLatestValue() <= ma_longterm_->GetLastValue() // 当前长期价格低于前价格
		&& ma_shortterm_->GetLatestValue() < ma_longterm_->GetLatestValue()  // 当前短期价格低于当前长期价格
		&& ma_shortterm_->GetLastValue() > ma_longterm_->GetLastValue()) // 当前长期价格高于前短期价格
	{
		action = SELL;

		//LOG("卖出, 日期: %s, 价格: %d, 当前金额: %.2f, 仓位: %.2f", quot.Date, quot.Close, amount_, position_);
	}

	return action;
}