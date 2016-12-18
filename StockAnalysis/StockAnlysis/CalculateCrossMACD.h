#pragma once

#include "FrameworkCls.h"

using namespace fmkcls;

class CalculateCrossMACD : public MultiThreadCalculator<CalculateCrossMACD>
{
	friend class MultiThreadCalculator<CalculateCrossMACD>;

public:
	CalculateCrossMACD(void);
	~CalculateCrossMACD(void);

public:

protected:
	virtual void Run();

	virtual void Done();

};
