#ifndef _HISTORY_DATA_EXTRACTOR_
#define _HISTORY_DATA_EXTRACTOR_

#include <vector>
#include "StockDataStructures.h"

using namespace std;

class HistoryDataExtractor
{
public:
	HistoryDataExtractor(void);
public:
	~HistoryDataExtractor(void);

	void download(const vector<StockProduct>& products);
};


#endif