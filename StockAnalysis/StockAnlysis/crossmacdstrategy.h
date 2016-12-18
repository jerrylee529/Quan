#pragma once

#include "strategy.h"
#include "macd.h"
#include "maline.h"
#include <boost/shared_ptr.hpp>

class CrossMACDStrategy :
	public Strategy
{
public:
	CrossMACDStrategy(const DataHandler& datahandler);
public:
	virtual ~CrossMACDStrategy(void);

public:
	virtual void Initialize();

	virtual ACTION Trigger(const Quot& quot);

	virtual void ReInitialize(const DataHandler& datahandler);

private:
	boost::shared_ptr<MACD> macd_;

	Quot last_quot_;
	int last_high_;

	bool search_buy_point;

	MACDItem last_macditem_;

    boost::shared_ptr<MALine> maline_;
};
