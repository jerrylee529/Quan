#pragma once

#include "Utilities.h"
#include "FrameworkCls.h"

using namespace fmkcls;

//
class CrossMAResultItem : public RowInfo
{
public:
	string GetCode() const
	{
		return fields[0];
	}

	int GetShortTerm() const
	{
		return convert<string, int>(fields[1]);
	}

	int GetLongTerm() const
	{
		return convert<string, int>(fields[2]);
	}

	double GetChange() const
	{
		return convert<string, double>(fields[3]);
	}

	double GetROI() const
	{
		return convert<string, double>(fields[4]);
	}

	// 最大收益 
	double GetMaxGain() const
	{
		return convert<string, double>(fields[5]);
	}

	// 最大回撤
	double GetMaxLoss() const
	{
		return convert<string, double>(fields[6]);
	}

	// 正收益次数
	int GetNumOfPosGain() const
	{
		return convert<string, int>(fields[7]);
	}

	//
	int GetNumOfBuy() const
	{
		return convert<string, int>(fields[8]);
	}
	
	//
	int GetNumOfSell() const
	{
		return convert<string, int>(fields[9]);
	}

	// 
	double GetRateOfPosGain() const
	{
		int numofgain = GetNumOfPosGain();
		
		int numofbuy = GetNumOfBuy();

		if (numofbuy > 0)
		{
			return (double)numofgain*100/numofbuy;
		}
		else
		{
			return 0;
		}
	}

	bool operator==(const CrossMAResultItem& value)
	{
		return (GetCode()==value.GetCode())?true:false;
	}

	bool Empty()
	{
		return (fields.size()<=0)?true:false;
	}
};

// 读取均线交叉计算结果 
class CrossMAResultReader
{
public:
	CrossMAResultReader(void);
	~CrossMAResultReader(void);
public:
	// 从文件中加载
	void Load(const char* filename);

	// 是否存在
	bool IsExisting(const char* code);

	// 找到一个
	CrossMAResultItem GetItem(const char* code);

	// 增加一个运算结果
	void AddItem(const CrossMAResultItem& item);

	// 清空
	void Clear();

	// 保存结果到指定文件
	void Save(const char* filename);

private:
	vector<CrossMAResultItem> items_;

	AccessLock lock_;
};
