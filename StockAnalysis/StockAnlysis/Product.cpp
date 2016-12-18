#include "StdAfx.h"
#include "Product.h"
#include "ZSDataLoader.h"
#include "MAStrategy.h"
#include "TrendStrategy.h"
#include "Configuration.h"
#include "weekline.h"
#include <set>
#include "TushareDataLoader.h"
#include <boost/date_time.hpp>

Product::Product(const char* code, const char* name)
:code_(code), name_(name)
{
}

Product::~Product(void)
{
	//
	quotations_.clear();
}

void Product::Load()
{
	assert(code_.length() > 0);

	//
	boost::gregorian::date d  = boost::gregorian::day_clock::local_day();

	// 读取行情数据
	string path = g_Configuration.getHitoryDataStorePath();
	path += "\\";
	//path += boost::gregorian::to_iso_extended_string(d);
	//path += "_";
	path += code_;
	path += ".csv";

	quotations_.clear();

	TushareDataLoader loader;
	loader.extract(path.c_str(), quotations_);
}

void Product::Save()
{
}

void Product::UpdateLatestQuot(const Quot& quot)
{
	lock_.Lock();

	// 写文件
    string strDate = GetEndDate();

	if (quot.getDate() == strDate)
	{
		quotations_[quotations_.size()-1] = quot;
	}
	else
	{
		quotations_.push_back(quot);
	}

	lock_.Unlock();
}

string Product::GetBeginDate()
{
	string str;

	if (quotations_.size() > 0)
	{
		str = quotations_[0].getDate();
	}

	return str;
}

string Product::GetEndDate()
{
	string str;

	if (quotations_.size() > 0)
	{
		str = quotations_[quotations_.size()-1].getDate();
	}

	return str;
}

double Product::GetIncreaseRate()
{
	if (quotations_.size() > 1)
	{
		int gain = quotations_[quotations_.size()-1].Close - quotations_[0].Open;

		return (double)gain*100/quotations_[0].Open;
	}
	else
	{
		return INVALID_VALUE;
	}
}
