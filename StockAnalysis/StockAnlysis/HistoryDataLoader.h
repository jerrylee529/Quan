#pragma once

#include "StockDataStructures.h"
#include <deque>
#include <map>
#include "Utilities.h"

using namespace std;

class HistoryDataLoader : public Observable
{
public:
	HistoryDataLoader(void);
public:
	virtual ~HistoryDataLoader(void);

	virtual void extract(const char* strFileName, vector<Quot>& stockTradeInfos) = 0;

protected:


};
