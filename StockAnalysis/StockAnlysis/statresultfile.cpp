#include "StdAfx.h"
#include "statresultfile.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "instrumentmanager.h"

using namespace std;

StatResultFile::StatResultFile(void)
{
}

StatResultFile::~StatResultFile(void)
{
}

void StatResultFile::Load(vector<StrategyResult>& results, const char* filename)
{
	string line, field;    //line为每行内容，field为每个字段

	//打开文件
	ifstream in;
	in.open(filename);    //以逗号隔开

	//处理文件内容
	if(in.is_open())
	{
		//循环取出每一行数据
		while(getline(in,line))
		{
			istringstream stream(line);

			//将一行的多个字段取出
			vector<string> rows;

			//以‘,’读取每个字段
			while(getline(stream, field, '	'))
			{
				//存入
				rows.push_back(field);
			}

			if (rows.size() < 10)
			{
				continue;
			}

			StrategyResult sr;
			
			results.push_back(sr);
		}

		// 关闭文件
		in.close();
	}
	else
	{
		//文件打开失败
		cout << "打开文件失败！";
	}
}

void StatResultFile::Save(const vector<StrategyResult>& results, const char* filename)
{
	// 写入文件
	ofstream os(filename, ios::out);

	if (os)
	{
		stringstream ssHeader;

		ssHeader<<"Code,Name,ROI,MaxGain,MaxLoss,NumOfPosGain,NumOfBuy,NumOfSell,Period,AveragePosROI,RateOfPosGain,LastBuyDate,CurrentStatus,AllBuy,AllSell,AllTrades";

	    os<<ssHeader.str()<<endl;

		ssHeader.clear();

		foreach(results, it, vector<StrategyResult>)
		{
			stringstream ss;

			ss<<it->code_<<","<<g_InstrumentMgr.GetInstrument(it->code_.c_str()).Name<<","<<CommonUtil::FormatDouble(it->GetROI())<<","<<CommonUtil::FormatDouble(it->GetMaxGain())
				<<","<<CommonUtil::FormatDouble(it->GetMaxLoss())
				<<","<<it->GetNumOfPosGain()<<","<<it->GetNumOfBuy()<<","<<it->GetNumOfSell() <<","<<it->GetPeriod()<<","
				<<CommonUtil::FormatDouble(it->GetAveragePosROI())<<","<<CommonUtil::FormatDouble(it->GetRateOfPosGain())
				<<","<<it->GetLastBuyDate() <<","<<it->GetCurrentStatus()<<","<<it->GetAllBuy()<<","<<it->GetAllSell()<<","<<it->GetAllTrades();

			os<<ss.str()<<endl;

			ss.clear();
		}

		os.close();
	}
}