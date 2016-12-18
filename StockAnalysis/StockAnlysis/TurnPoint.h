#pragma once
/********************************************************************
	created:	2015/06/04
	created:	4:6:2015   17:43
	filename: 	e:\Quan\StockAnlysis\StockAnlysis\TurnPoint.h
	file path:	e:\Quan\StockAnlysis\StockAnlysis
	file base:	TurnPoint
	file ext:	h
	author:		Jerry Li
	
	purpose:	计算转折点的类
*********************************************************************/

#include "algorithm.h"

//
struct TurnPointItem
{
	int direction; // 方向，0：上涨 1：下跌 2: 未知
	int close; // 价格
};


//
class TurnPoint :
	public Algorithm
{
public:
	TurnPoint(void);
public:
	~TurnPoint(void);

	// 趋势类型定义
	enum TREND{ UP = 0, DOWN, UNKNOWN};

	virtual void Calculate(const Quot& quot);

	// 得到当前的趋势
	TREND getCurrentTrend()  { return trend_; }

	// 设置价格变动的修正值，用于过滤微小的价格变化
	void setOffset(const int value) { offset_ = value; }

	// 得到当前的价格
	int close() { return close_; }

	//
	TurnPointItem GetTurnPoint(const char* date);

	//
	void AddTurnPoint(const char* date, const TurnPointItem& item);

private:
	TREND trend_;

	int offset_;

	int close_;

	string lastdate_;

	map<string, TurnPointItem> turnpoints_;
};
