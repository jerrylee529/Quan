#pragma once

#include "Strategy.h"
#include "Utilities.h"
#include "StockDataStructures.h"

// 策略计算类
class StrategyCalculator
{
public:
	StrategyCalculator(void);

	~StrategyCalculator(void);

public:
	void Calculate(const vector<Quot>& quots);	

	void ReCalculate(const vector<Quot>& quots, const DataHandler& datahandler);

	void Calculate(const Quot& quot);

	// 增加策略 
	void AddStrategy(const char* strategyname, const DataHandler& datahandler);

	// 删除策略
	void RemoveStrategy(const char* strategyname);

	// 清空策略
	void Clear();

	vector<string>& buyPoints() { return result_.buyPoints_; }

	vector<string>& sellPoints() { return result_.sellPoints_; }

	// 获得收益率
	virtual float GetROI();

	// 收益率序列
	vector<ROIInfo>& rois();

	// 最大收益 
	float GetMaxGain();

	// 最大回撤
	float GetMaxLoss();

	// 正收益次数
	int GetNumOfPosGain();

	// 买入次数
	int GetNumOfBuy();

	// 卖出次数
	int GetNumOfSell();

	// 得到交易次数
	int GetNumOfTrade();

	// 得到正收益率
	float GetRateOfPosGain();

	// 是否存在该策略
	bool IsExisting(const char* name);
	
	//
	vector<Strategy*>& strategies() { return strategies_; }

	//
	StrategyResult& result() { return result_; }

private:
	//
	vector<Strategy*> strategies_;

	// 当前仓位
	double position_;

	// 当前金额
	double amount_;

	//
	StrategyResult result_;
};

