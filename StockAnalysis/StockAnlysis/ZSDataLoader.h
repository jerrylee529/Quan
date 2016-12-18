#pragma once

#include "historydataloader.h"

class ZSDataLoader :
	public HistoryDataLoader
{
public:
	ZSDataLoader(void);
public:
	~ZSDataLoader(void);

	virtual void extract(const char* strFileName, vector<Quot>& stockTradeInfos);

private:
	int lastclose_;
};
