#pragma once
/********************************************************************
	created:	2015/05/14
	created:	14:5:2015   15:34
	filename: 	e:\Quan\StockAnlysis\StockAnlysis\Break20WeekKLine.h
	file path:	e:\Quan\StockAnlysis\StockAnlysis
	file base:	Break20WeekKLine
	file ext:	h
	author:		Jerry Li
	
	purpose:	突破20周线的算法
*********************************************************************/
#include "Algorithm.h"
#include <vector>

using namespace std;

enum DIRECTION
{
	UP,
	DOWN 
};
struct result
{
	DIRECTION direct;
	int percent;
};

class Break20WeekKLine :
	public Algorithm
{
public:
	Break20WeekKLine(void);
public:
	~Break20WeekKLine(void);

	void Calculate(const deque<Quot>& stockTradeInfos);

	void Export(const char* filename);

	void update(Observable* observable, void* arg);

	int GetUpPossibility();

	int GetDownPossibility();

	vector<result> VEC_Result;

	int totalTimes;
	int upTimes;
	int downTimes;
	int morethan5percent;
};
