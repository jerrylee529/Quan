#ifndef _CONFIGURATION_
#define _CONFIGURATION_

#include <string>
#include "Utilities.h"

using namespace std;


//
class Configuration : public Singleton<Configuration>, public Observable
{
	friend class Singleton<Configuration>;

public:
	// 从文件中读取数据
	void Load(const char* filename);

	// 写入文件
	void Save();

	// retrieve the url of downloading history data
	string getHistoryDataUrl(const char* strMarket, const char* strCode);

    // retrieve the path for saving the history data file
	string getHitoryDataStorePath();

	void SetHitoryDataStorePath(const char* value);

	// 获取代码文件路径
	string GetInstrumentFilePath();

	void SetInstrumentFilePath(const char* value);

	// 获取数据保存路径
	string GetSaveFilePath();

	void SetSaveFilePath(const char* value);

	// retrieves the cycle of mean
	int getMeanCycle();

	void setMeanCycle(const int value);

	// 
	int getAdjustCycle();

	void setAdjustCycle(const int value);

	//
	int getThreshold();

	void setThreshold(const int value);

	//
	int getStopLoss();

	void setStopLoss(const int value);

	// 获取趋势队列的大小
	int getTrendArraySize();

	//
	int safeMA_cycle(); 

	void setsafeMA_cycle(const int value);

	// 获取均线列表
	void GetMALines(vector<int>& mas);

	// 设置均线列表 
	void SetMALines(const vector<int>& mas);

	//
	int getOffset();

	void setOffset(const int value);

	//
	DataHandler& getDataHandler();

protected:
	Configuration(void);
	~Configuration(void);

private:
	int trendArraySize;

	DataHandler datahandler_;

	string filename_;
};

extern Configuration& g_Configuration;

#endif