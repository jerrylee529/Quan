#pragma once

#include "Strategy.h"

class StatResultFile
{
public:
	StatResultFile(void);
public:
	~StatResultFile(void);

	// 从文件中加载
	void Load(vector<StrategyResult>& results, const char* filename);

	// 保存结果到指定文件
	void Save(const vector<StrategyResult>& results, const char* filename);

};
