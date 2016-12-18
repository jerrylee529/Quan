#pragma once
#include "strategy.h"
#include "weekline.h"

class CrossWeekLineStrategy :
	public Strategy
{
public:
	CrossWeekLineStrategy(const DataHandler& datahandler);
	virtual ~CrossWeekLineStrategy(void);

public:
	virtual void Initialize();

	virtual ACTION Trigger(const Quot& quot);

private: 
	double amount_; // 金额

	double position_; // 当前仓位

	int latestClose_; // 最新的收盘价

	int lastVolume_;   // 上一个成交量

	int lastHigh_;  // 前一个最高点

	vector<Quot> week_line_data_;

	WeekLine* weekline_;
};
