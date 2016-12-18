#include "StdAfx.h"
#include "TrendMALine.h"

TrendMALine::TrendMALine(int cycle, int delta)
:MALine(cycle), latest_high_(INVALID_VALUE),latest_low_(INVALID_VALUE),detal_(delta),cycle_(cycle),
prevValue_(INVALID_VALUE),changed_(false),sumofclose_(0),quotNumber_(0),
sumofvol_(0), trend_(TurnPoint::UNKNOWN), date_of_latesthigh_(""), date_of_latestlow_("")
{
	turnPoint_.setOffset(delta);
}

TrendMALine::~TrendMALine(void)
{
	values.clear();
}

void TrendMALine::Calculate(const Quot& quot)
{
	quotNumber_++;

	// 计算转折点
	turnPoint_.Calculate(quot);

	Quot qt = quot;
	qt.Close = turnPoint_.close();

	values.push_back(qt);

    if (values.size() == cycle_)
	{
		foreach(values, it, deque<Quot>)
		{
			sumofclose_ += it->Close; 
			sumofvol_ += it->Volume;
		}
	}
	else if (values.size() > cycle_)
	{
		sumofclose_ -= values[0].Close;
		sumofvol_ -= values[0].Volume;
		sumofclose_ += values[values.size()-1].Close;
		sumofvol_ += values[values.size()-1].Volume;

		values.pop_front();
	}
	else
	{
		vtMAItem.push_back(INVALID_VALUE);

		return;
	}

	// 计算均价
	int avg = sumofclose_/cycle_;

	// 计算上一个高价
	if (latest_high_ != INVALID_VALUE)
	{
		if (avg > latest_high_)
		{
			latest_high_ = avg;

			date_of_latesthigh_ = quot.getDate();
		}
	}
	else
	{
		latest_high_ = avg;
	}

	// 计算上一个低价
	if (latest_low_ != INVALID_VALUE)
	{
		if (avg < latest_low_)
		{
			latest_low_ = avg;

			date_of_latestlow_ = quot.getDate();
		}
	}
	else
	{
		latest_low_ = avg;
	}

	// 计算转折点 
	switch (trend_)
	{
	case TurnPoint::UP:
		if (GetLatestClose() > avg) // 当前为上涨趋势， 如果收盘前比前面的收盘价低，则认为趋势变化
		{
			trend_ = TurnPoint::DOWN;

			TurnPointItem tpi;
			tpi.direction = trend_;
			tpi.close = avg;

			turnPoint_.AddTurnPoint(quot.Datetime, tpi);
		}
		break;
	case TurnPoint::DOWN:
		if (GetLatestClose() < avg) // 当前为下跌趋势， 如果收盘前比前面的收盘价低，则认为趋势变化
		{
			trend_ = TurnPoint::UP;

			TurnPointItem tpi;
			tpi.direction = trend_;
			tpi.close = avg;

			turnPoint_.AddTurnPoint(quot.Datetime, tpi);
		}
		break;
	case TurnPoint::UNKNOWN: 
		if (avg > GetLatestClose())
		{
			trend_ = TurnPoint::UP;

			TurnPointItem tpi;
			tpi.direction = trend_;
			tpi.close = avg;

			turnPoint_.AddTurnPoint(quot.Datetime, tpi);
		}
		else if (avg < GetLatestClose())
		{
			trend_ = TurnPoint::DOWN;

			TurnPointItem tpi;
			tpi.direction = trend_;
			tpi.close = avg;

			turnPoint_.AddTurnPoint(quot.Datetime, tpi);
		}

		break;
	default:
		break;
	}

	// 插入均价
	vtMAItem.push_back(avg);
}

int TrendMALine::GetLatestClose()
{
	if (!vtMAItem.empty())
	{
		return vtMAItem[vtMAItem.size()-1];
	}
	else
	{
		return INVALID_VALUE;
	}
}

int TrendMALine::GetLastClose()
{
	if (vtMAItem.size() > 1)
	{
		return vtMAItem[vtMAItem.size()-2];
	}
	else
	{
		return INVALID_VALUE;
	}
}

int TrendMALine::latest_high()
{
	return latest_high_;
}

int TrendMALine::latest_low()
{
	return latest_low_;
}

string TrendMALine::date_of_latesthigh()
{
	return date_of_latesthigh_;
}

string TrendMALine::date_of_latestlow()
{
	return date_of_latestlow_;
}