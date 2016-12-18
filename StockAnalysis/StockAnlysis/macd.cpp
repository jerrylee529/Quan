#include "StdAfx.h"
#include "macd.h"
#include <assert.h>
/*
1、计算移动平均值（EMA） 
12日EMA的算式为 
EMA（12）=前一日EMA（12）×11/13＋今日收盘价×2/13 
26日EMA的算式为 
EMA（26）=前一日EMA（26）×25/27＋今日收盘价×2/27 
2、计算离差值（DIF） 
DIF=今日EMA（12）－今日EMA（26） 
3、计算DIF的9日EMA 
根据离差值计算其9日的EMA，即离差平均值，是所求的MACD值。为了不与指标原名相混淆，此值又名DEA或DEM。 
今日DEA（MACD）=前一日DEA×8/10＋今日DIF×2/10 
计算出的DIF和DEA的数值均为正值或负值
*/

const int TRADE_LEFT = 0;
const int TRADE_RIGHT = 1;

MACD::MACD(const int shortterm, const int longterm, const int deaterm)
:shortterm_(shortterm), longterm_(longterm), deaterm_(deaterm), max_(-9999), min_(9999)
{
	ema_shortterm_ = new EMA(shortterm);
	ema_longterm_ = new EMA(longterm);
	ema_dea_ = new EMA(deaterm_);
	last_macd_ = INVALID_VALUE;
	name_ = "MACD";
	trade_direction_ = TRADE_LEFT;
	last_extremepoint_.macd = INVALID_VALUE;
}

MACD::~MACD(void)
{
	delete ema_shortterm_;
	delete ema_longterm_;
	delete ema_dea_;
}

void MACD::Calculate(const Quot& quot)
{
	double value = quot.getClose();

	MACDItem item;

	ema_shortterm_->Calculate(value);

	ema_longterm_->Calculate(value);

	double dif = ema_shortterm_->GetLatestValue() - ema_longterm_->GetLatestValue();  

	ema_dea_->Calculate(dif);

	double dea = ema_dea_->GetLatestValue();  

	item.dif = dif;
	item.dea = dea;
	item.macd = (dif - dea) * 2;

	max_ = max(max_, max(dif, dea));

	min_ = min(min_, min(dif, dea));

	items_.push_back(item);

	if (last_extremepoint_.macd <= INVALID_VALUE)
	{
		last_extremepoint_.macd = item.macd;
		last_extremepoint_.close = value;

		return;
	}

	//
	if (item.macd < 0)
	{
		if (last_extremepoint_.macd < 0)
		{
			if (last_extremepoint_.macd > item.macd)
			{
				last_extremepoint_.macd = item.macd;
				last_extremepoint_.close = quot.getLow();
			}
		}
		else
		{
			pos_extremepoints_.push_back(last_extremepoint_);

			last_extremepoint_.macd = item.macd;
			last_extremepoint_.close = value;

			extremepoints_.push_back(last_extremepoint_);
		}
	}
	else
	{
		if (last_extremepoint_.macd > 0)
		{
			if (last_extremepoint_.macd < item.macd)
			{
				last_extremepoint_.macd = item.macd;
				last_extremepoint_.close = quot.getHigh();
			}
		}
		else
		{
			neg_extremepoints_.push_back(last_extremepoint_);

			last_extremepoint_.macd = item.macd;
			last_extremepoint_.close = value;

			extremepoints_.push_back(last_extremepoint_);
		}
	}
}

double MACD::GetMax()
{
	return max_;
}

double MACD::GetMin()
{
	return min_;
}

MACDItem& MACD::getLastOne()
{
	assert(items_.size() > 1);

	return items_[items_.size()-2];	
}

MACDItem& MACD::getLatestOne()
{
	assert(items_.size() > 1);

	return items_[items_.size()-1];
}

void MACD::Reset()
{
	last_extremepoint_.macd = INVALID_VALUE;
	last_extremepoint_.close = INVALID_VALUE;

	pos_extremepoints_.clear();
	neg_extremepoints_.clear();
	extremepoints_.clear();
}

MACDItem& MACD::getThirdOneFromEnd()
{
	assert(items_.size() > 2);

	return items_[items_.size()-3];
}