#pragma once

#include "strategy.h"
#include "TrendMALine.h"
#include "maline.h"
#include "macd.h"

class TrendMACrossStrategy :
	public Strategy
{
public:
	TrendMACrossStrategy(const DataHandler& datahandler);
	virtual ~TrendMACrossStrategy(void);

public:
	virtual void Initialize();

	virtual ACTION Trigger(const Quot& quot);

	virtual void ReInitialize(const DataHandler& datahandler);

	MALine* GetMAShortTerm() { return maShortTerm; }

	TrendMALine* GetMALongTerm() { return maLongTerm; }

	// true: 表示上升趋势
	bool isUpTrend();

private: 
	MALine* maShortTerm;// 短期均线 

	TrendMALine* maLongTerm; // 长期均线

	MALine* maSafeTerm; // 保护均线 

	int threshold_; // 趋势线的波动幅度

	int lastHigh_; // 上一个最高价

	int shortterm_; // 短期均线 

	int longterm_;  // 长期均线

	MACD* macd_;
};
