#include "StdAfx.h"
#include "TrendStrategy.h"
#include "Configuration.h"

void TrendEventListener::DoOnBuy(const Quot& quot)
{

}

void TrendEventListener::DoOnSell(const Quot& quot)
{

}

//
TrendStrategy::TrendStrategy(const DataHandler& datahandler)
: Strategy(datahandler), buyPrice_(0), sellPrice_(0), 
gain_(0), prev_volume_(0),
maTrend(0), maLongTerm(0), first_low_(INVALID_VALUE), first_high_(INVALID_VALUE),
latesthigh_(INVALID_VALUE), latestlow_(INVALID_VALUE),
driftRate_(1.5), lastdate_("")
{
	prevHigh_.Close = INVALID_VALUE;
	prevLow_.Close = INVALID_VALUE;
	latestClose_ = INVALID_VALUE;

	trendterm_ = datahandler.GetItem<int>("tendterm", g_Configuration.getMeanCycle());
	threshold_ = datahandler.GetItem<int>("threshold", g_Configuration.getThreshold());
	safeterm_ = datahandler.GetItem<int>("safeterm", g_Configuration.getAdjustCycle());
}

TrendStrategy::~TrendStrategy(void)
{
	if (maTrend)
	{
		delete maTrend;
	}

	if (maLongTerm)
	{
		delete maLongTerm;
	}
}

void TrendStrategy::Initialize()
{
	maTrend = new TrendMALine(trendterm_, threshold_);

	maLongTerm = new MALine(safeterm_);

	name_ = "Trend";
}

Strategy::ACTION TrendStrategy::Trigger(const Quot& quot)
{
	// 计算过滤了波动的均线
	maTrend->Calculate(quot);

	// 计算长期保护均线
	maLongTerm->Calculate(quot);

	int latestclose = maTrend->GetLatestClose();

	// 如果波动均线为空则退出
	if (maTrend->getMAItems().empty() || maLongTerm->getMAItems().empty() || latestclose == INVALID_VALUE
		|| maTrend->date_of_latesthigh() == "" || maTrend->date_of_latestlow() == "")
	{
		lastdate_ = quot.getDate();

		return NONE;
	}

	ACTION action = NONE;

	TurnPointItem tpi = maTrend->turnpoints()->GetTurnPoint(quot.Datetime);

	if (tpi.close == INVALID_VALUE)
	{
		lastdate_ = quot.getDate();

		return NONE;
	}

	if (tpi.direction == 0)
	{
		latestlow_ = tpi.close;

		action = BUY;
	}
	else if (tpi.direction == 1)
	{
		latesthigh_ = tpi.close;

		action = SELL;
	}
	else
	{

	}

	return action;


	if (first_high_ != INVALID_VALUE)
	{
		if (!(action != BUY) && first_high_ > latesthigh_)
		{
			sellPrice_ = quot.Close;

			first_high_ = INVALID_VALUE;
		}
		else
		{
			first_high_ = latesthigh_;
		}
	}
	else
	{
		first_high_ = latesthigh_;
	}

	//
	if (first_low_ != INVALID_VALUE)
	{
		if (action != BUY && first_low_ < latestlow_ && latestlow_ > maLongTerm->GetLatestClose() && maLongTerm->GetLatestClose() != INVALID_VALUE)
		{
			buyPrice_ = quot.Close;

			first_low_ = INVALID_VALUE;
		}
		else
		{
			first_low_ = latestlow_;
		}
	}
	else
	{
		first_low_ = latestlow_;
	}

	lastdate_ = quot.getDate();

	return NONE;

	// 第一个高点
	if (first_high_ == INVALID_VALUE)
	{
		first_high_ = maTrend->latest_high();
	}
	else
	{
		// 如果当前高点
		if (maTrend->latest_high() > first_high_)
		{
			
		}
	}

	// 如果前面出现过买入点，之后再出现当前高点比前一个高点低的情况，则卖出
	string date = maTrend->date_of_latesthigh();

	if (latestclose*getDrift(date, quot.getDatetime()) < maTrend->latest_high() || latestclose < maLongTerm->GetLatestClose()) 
	{
		if (action != BUY)
		{
			sellPrice_ = quot.Close;
		}
	}

	// 如果前面出现过卖出点，之后出现一个低点比前一个低点高的情况，则买入
	date = maTrend->date_of_latestlow();

	if ((latestclose*getDrift(date, quot.getDatetime()) > maTrend->latest_low()) && maLongTerm->GetLatestClose() > latestclose)
	{
		if (action != SELL)
		{
			buyPrice_ = quot.Close;
		}
	}
}

TrendMALine* TrendStrategy::GetTrendMaLine()
{
	return maTrend;
}

int TrendStrategy::crossAvgLine(Quot& quot)
{
	vector<int>& avgs = maTrend->getMAItems();

	// 均线突破
	if (avgs.size() > 0)
	{
		int latestClose = avgs[avgs.size()-1];
		int lastClose = avgs[avgs.size()-2];

		if ((quot.Open <= latestClose) && (quot.Close > latestClose) && (latestClose > lastClose))
		{
			return BUY;
		}
		else if ((quot.Open > latestClose) && (quot.Close < latestClose) && (latestClose < lastClose))
		{
			return SELL;
		}
	}

	return NONE;
}

bool TrendStrategy::isNewHighVol(const Quot& quot)
{
	bool bRet = false;

	if (prev_volume_ != 0)
	{
		bRet = (quot.Volume/prev_volume_ >= 2)?true:false;
	}

	prev_volume_ = (int)quot.Volume;

	return bRet;
}

MALine* TrendStrategy::GetMALongTerm()
{
	return maLongTerm;
}

double TrendStrategy::getDrift(string begin, string end)
{
	Date dtBegin, dtEnd;
	dtBegin.fromStringYYYYMMDD(begin.c_str());
	dtEnd.fromStringYYYYMMDD(end.c_str());

	return driftRate_*(dtEnd-dtBegin);
}