#pragma once

#include "strategy.h"
#include "TrendMALine.h"
#include "maline.h"



//趋势事件跟踪
class TrendEventListener : public StrategyEventListener
{
public:
	virtual void DoOnBuy(const Quot& quot);

	virtual void DoOnSell(const Quot& quot);
};

// 趋势策略
class TrendStrategy :
	public Strategy
{
public:
	TrendStrategy(const DataHandler& datahandler);
	virtual ~TrendStrategy(void);

public:
	// 趋势类型定义
	enum TREND
	{
		TR_UP,
		TR_DOWN,
		TR_UNKNOWN
	};

	virtual void Initialize();

	virtual void ReInitialize(const DataHandler& datahandler) {}

	virtual ACTION Trigger(const Quot& quot);

	TrendMALine* GetTrendMaLine();

	MALine* GetMALongTerm();

private:
	void buy();

	void sell();

	int crossAvgLine(Quot& quot);

	bool isNewHighVol(const Quot& quot);

	//
	double getDrift(string begin, string end);

	int gain_;

	int buyPrice_;

	int sellPrice_;

	int prev_volume_;

	TREND currTrend_;  // 当前趋势

	string lastdate_;

	int first_high_;

	int first_low_;

	int latesthigh_;

	int latestlow_;

	Quot prevHigh_; // 前一个高点

	Quot prevLow_;  // 前一个低点 

	double driftRate_;

	int latestClose_; // 最新收盘价

	int last_close_; // 

	int last_high_;

	// 过滤微小波动的均线
	TrendMALine* maTrend;

	// 长均线保护
	MALine* maLongTerm;

	//
	int trendterm_; // 趋势线周期

	int safeterm_;  // 安全线周期

	int threshold_; // 波动阀值
};
