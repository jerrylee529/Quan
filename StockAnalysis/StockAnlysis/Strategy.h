#pragma once

#include "Utilities.h"
#include "StockDataStructures.h"

//
const double AMOUNT = 100000;

// ROI信息
struct ROIInfo
{
	int period; // 周期
	float roi;  // 收益率
};

// 交易信息
struct TradeInfo
{
	int period; // 周期
	float roi;  // 收益率
	string buy_date; // 买入日期
	string sell_date;// 卖出日期

	double buy_price; // 买入价格
	double sell_price; // 卖出价格
};

// 策略计算结果
struct StrategyResult
{
	// 股票代码
	string code_;

	// 总收益率
	float roi_;

	// 收益率列表
	vector<ROIInfo> rois_;

	// 买入点列表
	vector<string> buyPoints_;

	// 卖出点列表
	vector<string> sellPoints_;

	// 交易信息
	vector<TradeInfo> tradeInfos;

	// 收益率序列
	vector<ROIInfo>& rois()
	{
		return rois_;
	}

	StrategyResult(): roi_(0.0)
	{

	}

	//
	vector<TradeInfo>& GetTradeInfos()
	{
		return tradeInfos;
	}
	
	//
	string GetAllBuy() const
	{
		string str;

		foreach(buyPoints_, it, vector<string>)
		{
			str += *it;

			str += ",";
		}

		if (!str.empty())
		{
			str = str.substr(0, str.length() - 1);
		}

		return str;
	}

	//
	string GetAllSell() const
	{
		string str;

		foreach(sellPoints_, it, vector<string>)
		{
			str += *it;

			str += ",";
		}

		if (!str.empty())
		{
			str = str.substr(0, str.length() - 1);
		}

		return str;
	}

	//
	string GetAllTrades() const
	{
		string str;

		foreach(tradeInfos, it, vector<TradeInfo>)
		{
			stringstream ss;

			ss<<it->buy_date<<":"<<CommonUtil::FormatDouble(it->buy_price)<<":"<<it->sell_date<<":"<<CommonUtil::FormatDouble(it->sell_price)<<":"<<it->period<<":"<<CommonUtil::FormatDouble(it->roi);

			str += ss.str();

			str += ",";

			ss.clear();
		}

		if (!str.empty())
		{
			str = str.substr(0, str.length() - 1);
		}

		return str;
	}

	// 最大收益 
	float GetMaxGain() const
	{
		float roi = 0;

		if (!rois_.empty())
		{
			roi = rois_[0].roi;

			foreach(rois_, it, vector<ROIInfo>)
			{
				roi = max(roi, it->roi);
			}
		}

		return roi;

	}

	// 最大回撤
	float GetMaxLoss() const
	{
		float roi = 0;

		if (!rois_.empty())
		{
			roi = rois_[0].roi;

			foreach(rois_, it, vector<ROIInfo>)
			{
				roi = min(roi, it->roi);
			}
		}

		return roi;
	}

	// 正收益次数
	int GetNumOfPosGain() const
	{
		int ret = 0;

		foreach(rois_, it, vector<ROIInfo>)
		{
			if (it->roi > 0)
			{
				ret++;
			}
		}

		return ret;
	}

	// 计算正收益的平均周期
	int GetPeriod() const
	{
		int period = 0;
		int count = 0;

		if (!rois_.empty())
		{
			foreach(rois_, it, vector<ROIInfo>)
			{
				if (it->roi > 0)
				{
					period += it->period;
					count++;
				}
			}
		}

		if (count > 0)
		{
			return period/count;
		}
		else
		{
			return 0;
		}
	}

	// 得到最后一次买入的日期
	string GetLastBuyDate() const
	{
		if (buyPoints_.size() > 0)
		{
			return buyPoints_[buyPoints_.size()-1];
		}
		else
		{
			return "";
		}
	}

	// 计算正收益的平均值 
	double GetAveragePosROI() const
	{
		double roi = 0;
		int count = 0;

		if (!rois_.empty())
		{
			foreach(rois_, it, vector<ROIInfo>)
			{
				if (it->roi > 0)
				{
					roi += it->roi;
					count++;
				}
			}
		}

		if (count > 0)
		{
			return roi/count;
		}
		else
		{
			return 0;
		}
	}

	// 买入次数
	size_t GetNumOfBuy() const
	{
		return buyPoints_.size();
	}

	// 卖出次数
	size_t GetNumOfSell() const
	{
		return sellPoints_.size();
	}

	// 得到交易次数
	size_t GetNumOfTrade() const
	{
		return buyPoints_.size();
	}

	// 得到正收益率
	double GetRateOfPosGain() const
	{
		if (!buyPoints_.empty())
		{
			return (double)GetNumOfPosGain()*100/GetNumOfBuy();
		}
		else
		{
			return 0.0;
		}
	}

	//
	float GetROI() const
	{
		return roi_;
	}

	bool CanBuy() const
	{
		if (!buyPoints_.empty())
		{
			Date d = Date::Today();
			Date dateLastBuy;
			dateLastBuy.fromStringYYYYMMDD(buyPoints_[buyPoints_.size()-1].c_str());

			return (d == dateLastBuy)?true:false;
		}
		else
		{
			return false;
		}
	}

	bool CanSell() const
	{
		if (!buyPoints_.empty())
		{
			Date d = Date::Today();
			Date dateLastBuy;
			dateLastBuy.fromStringYYYYMMDD(buyPoints_[buyPoints_.size()-1].c_str());

			return (d == dateLastBuy)?true:false;
		}
		else
		{
			return false;
		}
	}

	bool CanHold() const
	{
		return buyPoints_.size()>sellPoints_.size()?true:false;
	}

	int GetCurrentStatus() const
	{
		int ret = -1;

		if (CanBuy())
		{
			ret = 0; //"买入"; 
		}
		else if (CanSell())
		{
			ret = 1; //"卖出";
		}
		else if (CanHold())
		{
			ret = 2; //"持有";
		}
		else
		{
			ret = 3; //"空仓";
		}

		return ret;
	}


};

//
class StrategyEventListener
{
public:
	virtual void DoOnBuy(const Quot& quot) = 0;

	virtual void DoOnSell(const Quot& quot) = 0;
};

//
class Strategy 
{
public:
	Strategy(const DataHandler& datahandler);
	virtual ~Strategy(void);

public:
	static Strategy* Create(const char* strategyname, const DataHandler& datahandler);


	// 动作类型定义
	enum ACTION
	{
		BUY,
		SELL,
		NONE
	};

	enum STATE
	{
		S_BUY,
		S_SELL,
		S_NONE
	};

	virtual void Initialize() = 0;
	
	virtual ACTION Trigger(const Quot& quot) = 0;

	virtual void ReInitialize(const DataHandler& datahandler) = 0;

    virtual void set_event_listener(StrategyEventListener* value)
	{
		event_listener_ = value;
	}

	// 设置止损点
	int stop_loss_point() { return stop_loss_point_; }

	void setstop_loss_point(const int value) { stop_loss_point_ = value; }

	// 策略名称
	string name() const { return name_; }

protected:
	void DoOnBuy(const Quot& quot);

	void DoOnSell(const Quot& quot);

	StrategyEventListener* event_listener_;

	int stop_loss_point_; // 止损点, 百分比

	string name_;

	int latestClose_; // 最新的收盘价

private:
};
