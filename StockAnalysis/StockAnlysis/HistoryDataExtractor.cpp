#include "StdAfx.h"
#include "HistoryDataExtractor.h"
#include "HttpDownloader.h"
#include "Utilities.h"
#include "Configuration.h"
#include <fstream>

using namespace std;

HistoryDataExtractor::HistoryDataExtractor(void)
{
}

HistoryDataExtractor::~HistoryDataExtractor(void)
{
}

void HistoryDataExtractor::download(const std::vector<StockProduct> &products)
{
	/*
    HttpDownloader downloader;
	string strUrl, strFile;

	for(vector<StockProduct>::const_iterator it = products.begin(); it != products.end(); it++)
	{
    	string strCode, strMarket;
	
		strCode.append(it->Code, sizeof(it->Code));
		strMarket.append(it->Market, sizeof(it->Market));

		strUrl = Singleton<Configuration>::getInstance().getHistoryDataUrl(strMarket.c_str(), strCode.c_str());

		strFile = Singleton<Configuration>::getInstance().getHitoryDataStorePath();

		strFile.append(it->Market, 2);
		strFile.append(it->Code, 6);

		strFile += ".csv";

		if (fileExists(strFile.c_str()))
		{
			continue;
		}

		if (downloader.download(strUrl.c_str(), strFile.c_str()))
		{
		    	
		}
	}
	*/
}