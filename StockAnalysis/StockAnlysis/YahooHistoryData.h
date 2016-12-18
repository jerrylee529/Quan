#ifndef _YAHOO_HISTORY_DATA_
#define _YAHOO_HISTORY_DATA_

#include "StockDataStructures.h"
#include "HistoryDataLoader.h"
#include <deque>

using namespace std;

class YahooHistoryData : public HistoryDataLoader
{
public:
	YahooHistoryData(void);
public:
	~YahooHistoryData(void);

	void extract(const char* strFileName, vector<Quot>& stockTradeInfos);
};


#endif