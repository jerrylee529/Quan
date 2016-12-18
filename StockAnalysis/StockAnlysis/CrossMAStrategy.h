#pragma once

#include "strategy.h"
#include "MALine.h"
#include "macd.h"

class CrossMAStrategy :
	public Strategy
{
public:
	CrossMAStrategy(const DataHandler& datahandler);
	virtual ~CrossMAStrategy(void);

public:
	virtual void Initialize();

	virtual ACTION Trigger(const Quot& quot);

	virtual void ReInitialize(const DataHandler& datahandler);

	void setVolumeIncrease(const int value) { volIncrease_ = value; }

	MALine* GetMAShortTerm() { return maShortTerm; }

	MALine* GetMALongTerm() { return maLongTerm; }

	// true: 表示上升趋势
	bool isUpTrend();

private: 
	MALine* maShortTerm;// 短期均线 

	MALine* maLongTerm; // 长期均线

	MALine* maSafeTerm; // 保护均线 

	int lastVolume_;   // 上一个成交量

	int volIncrease_;   // 成交量增加率

	int lastHigh_;  // 前一个最高点

	deque<int> trendArray_;// 最低价队列, 用于计算趋势

	int trendArraySize_; //趋势队列的大小

	int shortterm_; // 短期均线 

	int longterm_;  // 长期均线

	int safeterm_; // 保护均线

	MACDItem lastMACD_;

	MACDItem latestMACD_;

	int macdstatus_;

	MACD* macd_;
};
