#pragma once

#include "HistoryDataLoader.h"

class HistDataLoaderFactory
{
public:
	HistDataLoaderFactory(void);
public:
	~HistDataLoaderFactory(void);

	HistoryDataLoader* CreateDataLoader(UINT uType);
};
