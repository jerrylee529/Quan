#pragma once

/********************************************************************
	created:	2015/05/23
	created:	23:5:2015   18:34
	filename: 	e:\Quan\StockAnlysis\StockAnlysis\CalculateCrossMA.h
	file path:	e:\Quan\StockAnlysis\StockAnlysis
	file base:	CalculateCrossMA
	file ext:	h
	author:		Jerry Li
	
	purpose:	计算均线策略下个交易品种的最佳参数
*********************************************************************/

#include "FrameworkCls.h"
#include "Product.h"
#include "crossMA_result_reader.h"

using namespace fmkcls;

class CalculateCrossMA
{
public:
	CalculateCrossMA(HWND hwnd_progress, ShareQueue<Instrument>* instruments, CrossMAResultReader* reader);
	virtual ~CalculateCrossMA(void);

	// 运行
	void Execute();

	// 终止
	static void Terminate();

protected:
	static DWORD WINAPI CalculateStrategyProc(void* lpParam);

	static bool s_terminated_;

	static ShareVector<CalculateCrossMA*> s_threads_;  

	virtual void ExecuteStrategy(Product* product);

	HWND hwnd_progress_;
	string code_;
	ShareQueue<Instrument>* instruments_;
	CrossMAResultReader* reader_;
};
