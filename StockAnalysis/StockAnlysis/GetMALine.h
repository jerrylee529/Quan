#pragma once
/********************************************************************
	created:	2015/05/14
	created:	14:5:2015   15:44
	filename: 	e:\Quan\StockAnlysis\StockAnlysis\GetMALine.h
	file path:	e:\Quan\StockAnlysis\StockAnlysis
	file base:	GetMALine
	file ext:	h
	author:		Jerry Li
	
	purpose:	计算移动平均线的算法
*********************************************************************/

#include "algorithm.h"

// 计算移动平均线
class GetMALine :
	public Algorithm
{
public:
	GetMALine(int nCycle);
public:
	~GetMALine(void);

	void calculate(const Quot& quot);

	int getCycle() { return cycle_; }

	map<string, int> MAs;

private:
	int cycle_;
	int sumofclose_;
	int sumofvol_;

	deque<Quot> values;
};
