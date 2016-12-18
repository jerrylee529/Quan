#include "StdAfx.h"
#include "HistDataLoaderFactory.h"
#include "YahooHistoryData.h"
#include "ZSDataLoader.h"

HistDataLoaderFactory::HistDataLoaderFactory(void)
{
}

HistDataLoaderFactory::~HistDataLoaderFactory(void)
{
}

HistoryDataLoader* HistDataLoaderFactory::CreateDataLoader(UINT uType)
{
	HistoryDataLoader* pLoader = NULL;

    switch(uType)
	{
		case 0:
			pLoader = new ZSDataLoader();
			break;
		case 1:
			pLoader = new YahooHistoryData();
			break;
		default:
			break;
	}

	return pLoader;
}