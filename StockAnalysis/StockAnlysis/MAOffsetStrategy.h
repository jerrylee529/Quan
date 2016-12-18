#pragma once

#include "strategy.h"
#include "maline.h"

class MAOffsetStrategy :
	public Strategy
{
public:
	MAOffsetStrategy(const DataHandler& datahandler);
public:
	virtual ~MAOffsetStrategy(void);

public:
	virtual void Initialize();

	virtual ACTION Trigger(const Quot& quot);

	virtual void ReInitialize(const DataHandler& datahandler);

	MALine* GetMALine() { return maLine_; }


private:
	MALine* maLine_;

	int lastClose_; // 最新的收盘价

	int latestLow_; // 最近的最低价

	int lastHigh_;
};