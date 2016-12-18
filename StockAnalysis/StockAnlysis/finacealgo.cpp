#include "StdAfx.h"
#include "financealgo.h"
#include "Utilities.h"

const int INVALID_VALUE = -9999;

namespace finance_algo
{
	MA::MA(int cycle)
		:cycle_(cycle),sumofvalue_(0.0)
	{
	}

	MA::~MA(void)
	{
	}

	void MA::Calculate(const double value)
	{
		values_.push_back(value);

		if (values_.size() == cycle_)
		{
			foreach(values_, it, deque<double>)
			{
				sumofvalue_ += *it; 
			}
		}
		else if (values_.size() > cycle_)
		{
			sumofvalue_ -= values_[0];
			sumofvalue_ += values_[values_.size()-1];

			values_.pop_front();
		}
		else
		{
			items_.push_back(INVALID_VALUE);

			return;
		}

		double avg = sumofvalue_/cycle_;

		items_.push_back(avg);
	}

	double MA::GetLatestValue()
	{
		if (!items_.empty())
		{
			return items_[items_.size()-1];
		}
		else
		{
			return INVALID_VALUE;
		}
	}

	double MA::GetLastValue()
	{
		if (items_.size() > 2)
		{
			return items_[items_.size()-2];
		}
		else
		{
			return INVALID_VALUE;
		}
	}

	vector<double>& MA::items()
	{
		return items_;
	}

	//
	float GetTrendLineRate(const float begin, const float end, const int timespan)
	{
		float f = 0.0;

		f = ((end - begin)/begin)*100/timespan;

		return f;
	}

}
