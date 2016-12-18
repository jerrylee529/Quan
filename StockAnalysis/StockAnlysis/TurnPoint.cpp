#include "StdAfx.h"
#include "TurnPoint.h"

TurnPoint::TurnPoint(void)
:trend_(UNKNOWN), close_(INVALID_VALUE), lastdate_("")
{
}

TurnPoint::~TurnPoint(void)
{
}

void TurnPoint::Calculate(const Quot& quot)
{
	// 如果当前没有任何数据点则退出
	if (close_ == INVALID_VALUE)
	{
		close_ = quot.Close;

		lastdate_ = quot.getDate();

		return;
	}

	// 如果修正过的价格在offset定义的范围内，则认为价格未变化
	if (abs(quot.Close - close_)*100/close_ <= offset_)
	{
		lastdate_ = quot.getDate();

		return;
	}

/*
	// 根据各种趋势进行处理
	switch (trend_)
	{
	case UP:
		if (quot.Close < close_) // 当前为上涨趋势， 如果收盘前比前面的收盘价低，则认为趋势变化
		{
			trend_ = DOWN;

			TurnPointItem tpi;
			tpi.direction = DOWN;
			tpi.close = close_;

			turnpoints_.insert(make_pair(quot.getDate(), tpi));
		}
		break;
	case DOWN:
		if (quot.Close > close_) // 当前为下跌趋势， 如果收盘前比前面的收盘价低，则认为趋势变化
		{
			trend_ = UP;

			TurnPointItem tpi;
			tpi.direction = UP;
			tpi.close = close_;

			turnpoints_.insert(make_pair(quot.getDate(), tpi));
		}
		break;
	case UNKNOWN: 
		if (quot.Close > close_)
		{
			trend_ = UP;

			TurnPointItem tpi;
			tpi.direction = UP;
			tpi.close = close_;

			turnpoints_.insert(make_pair(quot.getDate(), tpi));
		}
		else if (quot.Close < close_)
		{
			trend_ = DOWN;

			TurnPointItem tpi;
			tpi.direction = DOWN;
			tpi.close = close_;

			turnpoints_.insert(make_pair(quot.getDate(), tpi));
		}

		break;
	default:
		break;
	}
*/

	close_ = quot.Close;

	lastdate_ = quot.getDate();
}

TurnPointItem TurnPoint::GetTurnPoint(const char* date)
{
	TurnPointItem item;
	item.direction = UNKNOWN;
	item.close = INVALID_VALUE;

	map<string, TurnPointItem>::iterator it = turnpoints_.find(string(date));

	if (it != turnpoints_.end())
	{
		item = it->second;
	}

	return item;
}

void TurnPoint::AddTurnPoint(const char* date, const TurnPointItem& item)
{
	turnpoints_.insert(make_pair(string(date), item));
}