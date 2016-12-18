#pragma once

#include "historydataloader.h"

class TushareDataLoader :
	public HistoryDataLoader
{
public:
	TushareDataLoader(void);
public:
	virtual ~TushareDataLoader(void);

	virtual void extract(const char* strFileName, vector<Quot>& stockTradeInfos);

private:
	int lastclose_;
};
