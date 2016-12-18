#include "StdAfx.h"
#include "Configuration.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>

using namespace std;

Configuration& g_Configuration = Configuration::getInstance();

const string CONFIGURATION_FILE="config.txt";

const string HIST_DATA_PATH_KEY = "history_data_path"; // 历史数据路径 
const string INSTRUMENT_FILE_PATH_KEY = "instrument_file_path";// 代码文件路径
const string SAVE_FILE_PATH_KEY = "save_file_path";// 品种数据文件路径
const string SHORT_TERM_KEY = "short_term";  // 短期均线
const string LONG_TERM_KEY = "long_term"; // 长期均线
const string THRESHOLD_KEY = "threshold"; // 趋势线波动幅度 
const string STOP_LOSS_KEY = "stop_loss_rate"; // 止损比例
const string SAFE_TERM_KEY = "safe_term"; //保护均线
const string MA_LINE_KEY = "ma_lines"; //均线
const string MA_OFFSET = "ma_offset"; //均线偏移比例

const char ARRAY_DELIMITER = ',';


Configuration::Configuration(void)
: trendArraySize(3)
{
}

Configuration::~Configuration(void)
{
}

void Configuration::Load(const char* filename)
{
	assert(filename != NULL);

	string filepath;

	if (strlen(filename) == 0)
	{
		char buf[MAX_PATH+1] = {0};
		GetModuleFileName(NULL,buf,MAX_PATH);
		int m_iPosIndex;
		string fullfilepath(buf);
		int iPos = fullfilepath.rfind('\\');
		filepath = fullfilepath.substr(0, iPos);
		filepath += "\\" + CONFIGURATION_FILE;
	}
	else
	{
		filepath = filename;
	}

	filename_ = filepath;

	datahandler_.Load(filepath.c_str());
}

void Configuration::Save()
{
	datahandler_.Save(filename_.c_str());

	notifyObservers((void*)&datahandler_);
}

string Configuration::getHistoryDataUrl(const char* strMarket, const char* strCode)
{
	string strUrl = "http://table.finance.yahoo.com/table.csv?s=";

	strUrl += strCode;

	strUrl += ".";

	strUrl += strMarket;

	return strUrl;
}

string Configuration::getHitoryDataStorePath()
{
	return datahandler_.GetItem<string>(HIST_DATA_PATH_KEY, "E:\\Stock\\TEST\\");
}

void Configuration::SetHitoryDataStorePath(const char* value)
{
	datahandler_.SetItem<string>(HIST_DATA_PATH_KEY, value);
}

int Configuration::getMeanCycle()
{
	return datahandler_.GetItem<int>(SHORT_TERM_KEY, 10);
}

void Configuration::setMeanCycle(const int value)
{
	datahandler_.SetItem<int>(SHORT_TERM_KEY, value);
}

int Configuration::getAdjustCycle()
{
	return datahandler_.GetItem<int>(LONG_TERM_KEY, 60);
}

void Configuration::setAdjustCycle(const int value)
{
	datahandler_.SetItem<int>(LONG_TERM_KEY, value);
}

int Configuration::getThreshold()
{
	return datahandler_.GetItem<int>(THRESHOLD_KEY, 5);
}

void Configuration::setThreshold(const int value)
{
	datahandler_.SetItem<int>(THRESHOLD_KEY, value);
}

int Configuration::getStopLoss()
{
	return datahandler_.GetItem<int>(STOP_LOSS_KEY, 5);
}

void Configuration::setStopLoss(const int value)
{
	datahandler_.SetItem<int>(STOP_LOSS_KEY, value);
}

int Configuration::getTrendArraySize()
{
	return trendArraySize;
}

int Configuration::safeMA_cycle()
{
	return datahandler_.GetItem<int>(SAFE_TERM_KEY, 250);
}

void Configuration::setsafeMA_cycle(const int value)
{
	datahandler_.SetItem<int>(SAFE_TERM_KEY, value);
}

string Configuration::GetInstrumentFilePath()
{
	return datahandler_.GetItem<string>(INSTRUMENT_FILE_PATH_KEY, "E:\\Stock\\");
}

void Configuration::SetInstrumentFilePath(const char* value)
{
	datahandler_.SetItem<string>(INSTRUMENT_FILE_PATH_KEY, value);
}

string Configuration::GetSaveFilePath()
{
	return datahandler_.GetItem<string>(SAVE_FILE_PATH_KEY, "E:\\Stock\\product\\");
}

void Configuration::SetSaveFilePath(const char* value)
{
	datahandler_.SetItem<string>(SAVE_FILE_PATH_KEY, value);
}

void Configuration::GetMALines(vector<int>& mas)
{
	datahandler_.GetArray<int>(MA_LINE_KEY, mas, ARRAY_DELIMITER);
}

void Configuration::SetMALines(const vector<int>& mas)
{
	datahandler_.SetArray<int>(MA_LINE_KEY, mas, ARRAY_DELIMITER);
}

int Configuration::getOffset()
{
	return datahandler_.GetItem<int>(MA_OFFSET, 40);
}

void Configuration::setOffset(const int value)
{
	datahandler_.SetItem<int>(MA_OFFSET, value);
}

DataHandler& Configuration::getDataHandler()
{
	return datahandler_;
}