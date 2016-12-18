#pragma once

#include <map>
#include <string>
#include <vector>
#include "FrameworkCls.h"
#include "StockDataStructures.h"
#include "Utilities.h"
#include "Strategy.h"
#include "maline.h"

using namespace std;
using namespace fmkcls;


// 品种
class Product : public Observable
{
public:
	Product(const char* code, const char* name = "");
	~Product(void);

public:
	// 读取数据
	void Load();

	// 保存数据
	void Save();

	// 股票代码
	string code() { return code_; }

	//
	string name() { return name_; }

	vector<Quot>& GetQuotations() { return quotations_; }

	// 获取行情开始日期
	string GetBeginDate();

	// 获取行情结束日期
	string GetEndDate();

	// 获取投资收益率
	double GetIncreaseRate(); 

	// 更新最新行情
	void UpdateLatestQuot(const Quot& quot);

protected:

private:
	// 股票代码
	string code_;

	string name_;

	// 行情列表
	vector<Quot> quotations_;

	AccessLock lock_;

	DataHandler datahandler_;
};
