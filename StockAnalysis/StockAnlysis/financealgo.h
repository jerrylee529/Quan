#pragma once

#include <vector>
#include <deque>

using namespace std;

namespace finance_algo
{
	// 均线算法 
	class MA
	{
	public:
		MA(int cycle);

		~MA(void);

	public:
		void Calculate(const double value);

		int cycle() { return cycle_; }

		double GetLatestValue(); // 获得最后一个均价

		double GetLastValue(); // 获得前一个均价

		vector<double>& items();

	private:
		int cycle_;

		double sumofvalue_;

		vector<double> items_;

		deque<double> values_;
	};

	// 趋势线斜率计算
	/*
	begin: 开始价格
	end: 结束价格
    timespan: 时间跨度
	*/
	float GetTrendLineRate(const float begin, const float end, const int timespan);
}
