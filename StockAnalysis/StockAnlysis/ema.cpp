#include "StdAfx.h"
#include "ema.h"

EMA::EMA(int cycle)
:cycle_(cycle)
{
}

EMA::~EMA(void)
{
}

void EMA::Calculate(const double value)
{
	if (items_.empty())
	{
		items_.push_back(value);

		return;
	}

	double d = (2 * value + (cycle_ - 1) * items_[items_.size()-1]) / (cycle_ + 1);

	items_.push_back(d);
}

double EMA::GetLatestValue()
{
	return items_[items_.size() - 1];
}