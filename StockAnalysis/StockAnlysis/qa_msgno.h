#ifndef _QA_MSGNO_h
#define _QA_MSGNO_h

#include "StockDataStructures.h"
#include <map>

using namespace std;

// 策略计算完毕时发送
const int QAM_STRATEGY_DONE = WM_USER + 1;

// 设置进度条范围
const int QAM_PROGRESS_SETRANGE = WM_USER + 2;

// 设置进度条位置
const int QAM_PROGRESS_SETPOS = WM_USER + 3;

// 设置进度完成
const int QAM_PROGRESS_DONE = WM_USER + 4;

// 更新行情
const int QAM_QUOTATION_LATEST = WM_USER + 5;

// 品种买卖信息
const int QAM_PRODUCT_TRADE = WM_USER + 6;

// 品种卖出信息


// 更新进度条
struct MsgProgressSetRange
{
	string title;
	int minimize;
	int maximize;
};

// 更新进度
struct MsgProgressSetPos
{
	string prompt;
	int pos;
};

// 进度完成
struct MsgProgressDone
{
	string prompt;
};

// 更新策略计算的结果
struct MsgStrategyDone
{
	string code;   // 股票代码
	double roi;    // 收益率
	int shortterm; // 短期均线
	int longterm;  // 长期均线
};

// 行情接收
struct MsgRecvQuotation
{
	map<string, Quot> quotations; //code: 股票代码, quot: 行情
};

// 品种买卖消息
struct MsgProductTrade
{
	unsigned char flag; // 0: 买入 1: 卖出
	string code; // 股票代码
    Quot quot;   // 行情
};

#endif