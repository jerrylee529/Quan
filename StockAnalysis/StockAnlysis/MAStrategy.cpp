#include "StdAfx.h"
#include "MAStrategy.h"
#include "Utilities.h"
#include "Configuration.h"
#include "TrendMALine.h"


MAStrategy::MAStrategy(const DataHandler& datahandler)
: Strategy(datahandler), lastClose_(INVALID_VALUE), latestLow_(INVALID_VALUE),macd_(0),
lastHigh_(INVALID_VALUE)
{

}

MAStrategy::~MAStrategy(void)
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

void MAStrategy::Initialize()
{
	maLine_ = new TrendMALine(g_Configuration.getMeanCycle(), g_Configuration.getThreshold());
	maLongTerm = new MALine(g_Configuration.getAdjustCycle());

	macd_ = new MACD(12, 26, 9);

	name_ = "MA";
}

Strategy::ACTION MAStrategy::Trigger(const Quot& quot)
{
	//
	macd_->Calculate(quot);

	// 计算均线
	maLine_->Calculate(quot);

	// 计算长期保护均线
	maLongTerm->Calculate(quot);

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

	// 得到最新的均线价格
	int maClose =  maLine_->GetLatestClose();

	// 无均线价格时退出
	if (maClose == INVALID_VALUE)
	{
		lastClose_ = quot.Close;

		return NONE;
	}

	ACTION action = NONE;

	int nStopLossRate = g_Configuration.getStopLoss();

	// 如果当前收盘价小于或者等于均线价格，则准备抛出
	if ((((quot.High > maClose && quot.Close <= maClose)
		|| (maLine_->GetLastClose() <= lastClose_ && quot.Close < maLine_->GetLatestClose()))
		&& quot.Close < quot.Open)
		/*|| change >= nStopLossRate*/)
	{
		lastHigh_ = INVALID_VALUE;

		action =   SELL;
	}
	else 
	{
		// 如果当前没有仓位则买入，要求均线价格在长期保护均线之上
		if (((quot.Close > maClose && quot.Low <= maClose) // 收盘价高于均线价格
			|| (maLine_->GetLastClose() >= lastClose_ && quot.Close > maLine_->GetLatestClose()))
			&& quot.Close > quot.Open
			) 
		{
			lastHigh_ = quot.High;

			action = BUY;
		}
	}

	lastClose_ = quot.Close;

	return action;
}

void MAStrategy::ReInitialize(const DataHandler& datahandler)
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

	maLine_ = new TrendMALine(shortterm_, g_Configuration.getThreshold());
	maLongTerm = new MALine(longterm_);

	values.clear();
}