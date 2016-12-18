#pragma once

#include "Strategy.h"
#include "maline.h"
#include "macd.h"

class ReboundStrategy : public Strategy
{
public:
	ReboundStrategy(const DataHandler& datahandler);
	~ReboundStrategy(void);

public:
	virtual void Initialize();

	virtual ACTION Trigger(const Quot& quot);

	virtual void ReInitialize(const DataHandler& datahandler);

	MALine* GetMALine() { return maLine_; }

	MALine* GetMALongTerm() { return maLongTerm; }

private:
	MALine* maLine_;

	MALine* maLongTerm; // 长期保护均线

	int lastClose_; // 最新的收盘价

	int latestLow_; // 最近的最低价

	deque<int> values;

	MACD* macd_;

	int lastHigh_;};
