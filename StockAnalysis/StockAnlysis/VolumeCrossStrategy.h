#pragma once

#include "Strategy.h"
#include "financealgo.h"

using namespace finance_algo;

class VolumeCrossStrategy : public Strategy
{
public:
	VolumeCrossStrategy(const DataHandler& datahandler);
	~VolumeCrossStrategy(void);

public:
	virtual void Initialize();

	virtual void ReInitialize(const DataHandler& datahandler) {}

	virtual ACTION Trigger(const Quot& quot);

private: 
	int lastHigh_; // 上一个最高价

	int shortterm_; // 短期均线 

	int longterm_;  // 长期均线

	MA* ma_shortterm_;

	MA* ma_longterm_;
};
