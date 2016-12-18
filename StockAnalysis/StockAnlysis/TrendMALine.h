#pragma once

#include "algorithm.h"
#include "TurnPoint.h"
#include "maline.h"


class TrendMALine :
	public MALine
{
public:
	TrendMALine(int cycle, int delta);
public:
	~TrendMALine(void);

	// 计算
	virtual void Calculate(const Quot& quot);

	// 得到最新价
	int GetLatestClose();

	// 得到上一个价格
	int GetLastClose();

	// 得到上一个最高价
	int latest_high();

	// 得到一个最低价
	int latest_low();

	// 上一个高点的日期
	string date_of_latesthigh();

	// 上一个低点的日期
	string date_of_latestlow();

	// 趋势转变
	bool getChanged() { return changed_; }

	TurnPoint* turnpoints() { return &turnPoint_; }

	int cycle() { return cycle_; }

private:
	int latest_high_;
	int latest_low_;
	string date_of_latesthigh_;
	string date_of_latestlow_;
	int detal_;
	int cycle_;
	int sumofclose_;
	int sumofvol_;
	int firstValue;
	int prevValue_;
	bool changed_;
	int quotNumber_;
	int trend_;

	TurnPoint turnPoint_;
};
