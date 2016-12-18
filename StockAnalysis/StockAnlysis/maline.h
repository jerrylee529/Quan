#pragma once
/********************************************************************
	created:	2015/05/14
	created:	14:5:2015   15:44
	filename: 	e:\Quan\StockAnlysis\StockAnlysis\MALine.h
	file path:	e:\Quan\StockAnlysis\StockAnlysis
	file base:	MALine
	file ext:	h
	author:		Jerry Li
	
	purpose:	计算移动平均线的算法
*********************************************************************/

#include "algorithm.h"

// 计算移动平均线
class MALine : public Algorithm
{
public:
	MALine(int nCycle);

	virtual ~MALine(void);

public:
	enum MATRENDTYPE
	{
		MA_UP,
		MA_DOWN,
		MA_UNKNOWN
	};

	virtual void Calculate(const Quot& quot);

	int cycle() const { return cycle_; }

	int GetLatestClose(); // 获得最后一个均价

	int GetLastClose(); // 获得前一个均价

	int GetCurrTrendType();

	bool isUp() { return (GetCurrTrendType()==MA_UP)?true:false; }

	vector<int>& getMAItems();

protected:
	int cycle_;
	int sumofclose_;
	int sumofvol_;

	MATRENDTYPE maTrendType_;

	vector<int> vtMAItem;

	deque<Quot> values;
};
