#pragma once

#include "algorithm.h"
#include "Strategy.h"

class NewHighPrice :
	public Strategy
{
public:
	NewHighPrice(const DataHandler& datahandler);
	~NewHighPrice(void);

public:
	virtual void Initialize();

	virtual ACTION Trigger(const Quot& quot);

	virtual void ReInitialize(const DataHandler& datahandler);

	int GetValue() { return newHighPrice; }

	bool IsNewHighPrice();
private:
	int m_cycle;
	int newHighPrice;
	deque<int> values;
};
