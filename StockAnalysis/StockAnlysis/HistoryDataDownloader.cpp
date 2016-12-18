#include "StdAfx.h"
#include "HistoryDataDownloader.h"
#include "HttpDownloader.h"
#include "Utilities.h"
#include "Configuration.h"
#include <fstream>

HistoryDataDownloader::HistoryDataDownloader(void)
{
}

HistoryDataDownloader::~HistoryDataDownloader(void)
{
}

bool fileExists(const char* strFileName)
{
	ifstream fin(strFileName);
	if (!fin)
	{
		return false;
	}
	else
	{
		return true;
	}

}

void HistoryDataDownloader::download(const char* market, const char* code)
{
	string strCode = code;
		
	string strMarket = market;

	string strUrl = Singleton<Configuration>::getInstance().getHistoryDataUrl(strMarket.c_str(), strCode.c_str());

	string strFile = Singleton<Configuration>::getInstance().getHitoryDataStorePath() + strMarket + strCode + ".csv";

	/*
	if (fileExists(strFile.c_str()))
	{
		m_strDownloadFile = strFile;

		return;
	}
	*/

	HttpDownloader downloader;

	if (downloader.download(strUrl.c_str(), strFile.c_str()))
	{
		m_strDownloadFile = strFile;
	}
}