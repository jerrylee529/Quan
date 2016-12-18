#pragma once
/********************************************************************
	created:	2015/05/23
	created:	23:5:2015   14:18
	filename: 	e:\Quan\StockAnlysis\StockAnlysis\macd.h
	file path:	e:\Quan\StockAnlysis\StockAnlysis
	file base:	macd
	file ext:	h
	author:		Jerry Li
	
	purpose:	计算MACD
*********************************************************************/

#include "ema.h"
#include "indicator.h"

//
struct MACDItem
{
	double dif;
	double dea;
	double macd;
};

//
struct MACDExtremePoint
{
	double close;
	double macd;
};

//
class MACD : public Indicator
{
public:
	MACD(const int shortterm, const int longterm, const int deaterm);

	virtual ~MACD(void);

public:
	virtual void Calculate(const Quot& quot);

	MACDItem& getLatestOne(); // 获得最后一个

	MACDItem& getLastOne(); // 获得前一个

	int getCurrTrendType();

	double GetMax();

	double GetMin();

	void Reset();

	vector<MACDItem>& items() { return items_; }

	int LongTerm() { return longterm_; }

	int ShortTerm() { return shortterm_; }

	int DeaTerm() { return deaterm_; }

	vector<MACDExtremePoint>& GetPosExtremePoints() { return pos_extremepoints_; }

	vector<MACDExtremePoint>& GetNegExtremePoints() { return neg_extremepoints_; }

	vector<MACDExtremePoint>& GetExtremePoints() { return extremepoints_; }

	size_t GetCount() { return items_.size(); }

	// 得到倒数第三个
	MACDItem& getThirdOneFromEnd();

private:
	int longterm_;
	int shortterm_;
	int deaterm_;

	double max_;

	double min_;

	double last_macd_;

	MACDExtremePoint last_extremepoint_;

	int trade_direction_; // 0: left 1: right

	vector<MACDExtremePoint> pos_extremepoints_;

	vector<MACDExtremePoint> neg_extremepoints_;

	vector<MACDExtremePoint> extremepoints_;

	vector<MACDItem> items_;

	EMA* ema_dea_;
	EMA* ema_shortterm_;
	EMA* ema_longterm_;
};
