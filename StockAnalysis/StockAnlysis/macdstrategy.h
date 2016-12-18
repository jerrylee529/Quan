#pragma once

#include "Strategy.h"
#include "macd.h"

class MACDStrategy : public Strategy
{
public:
	MACDStrategy(const DataHandler& datahandler);
	virtual ~MACDStrategy(void);

public:
	virtual void Initialize();

	virtual ACTION Trigger(const Quot& quot);

	virtual void ReInitialize(const DataHandler& datahandler);

private:
	MACD* macd_;

	double last_macd_;

	int buyPrice;

	int lowPrice_; // 买入时的最低价
};
