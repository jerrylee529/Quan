#include "StdAfx.h"
#include "StockPicker.h"
#include "HistoryDataDownloader.h"
#include "StockProductData.h"
#include "Configuration.h"
#include "Utilities.h"
#include "StockAnalyst.h"
#include "YahooHistoryData.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

StockPicker::StockPicker(void)
{
	Configuration& config = Singleton<Configuration>::getInstance();
    m_cycle = config.getAdjustCycle();
	m_downloadInterval = config.getDownloadInterval();
	m_isRunning = false;
	m_hQuitHandle = NULL;
	m_progess = NULL;
}

StockPicker::~StockPicker(void)
{
}

void StockPicker::pickup()
{
	time_t t = time(NULL);
	struct tm *local = localtime(&t);

	char filename[256] = {0};
	sprintf(filename, "H:\\Quan\\pickedstock_%04d%02d%02d.txt", local->tm_year+1900, local->tm_mon+1, local->tm_mday);

	ofstream o_file;
	string out_text;

	// open the pickedstock.txt
	o_file.open(filename);

	int value = m_cycle;

	for (vector<StockProduct>::const_iterator it = m_stockProductData.stockProducts.begin(); 
		it != m_stockProductData.stockProducts.end(); it++)
	{
		StockProduct sp = *it;
        
		// download the day K line data
		HistoryDataDownloader hdd;

		log("Download stock data, market: %s, code: %s", sp.getMarket().c_str(), sp.getCode().c_str());

		hdd.download(sp.getMarket().c_str(), sp.getCode().c_str());

		string strFileName = hdd.getDownloadFile();
		
		if (strFileName.empty())
		{
			continue;
		}

		// extract the data from K line file
		log("Extrace stock data from %s", strFileName.c_str());

		YahooHistoryData yhd;
		yhd.extract(strFileName.c_str(), sp.getMarket().c_str(), sp.getCode().c_str());

		// calculate the week line
		log("%s", "calculate week line");

		map<string, StockTradeInfo> weekLines;

		StockAnalyst sa;
		sa.calculateWeekLine(yhd.stockTradeInfos, weekLines);

		// calculate if the close is higher than previous cycle days
		log("calculate break point, cycle is %d", m_cycle);

		if (sa.calcuateBreakPoint(m_cycle, weekLines))
		{
			log("Market: %s, Code: %s break successfully", sp.getMarket().c_str(), sp.getCode().c_str());

			m_pickedStockProducts.push_back(sp);

			o_file << "Market:" << sp.getMarket() << ", Code:" <<sp.getCode()<<endl; 
		}

		if (WaitForSingleObject(m_hQuitHandle, m_downloadInterval*1000) == WAIT_OBJECT_0)
		{
			log("Stop pick up stock");

			break;
		}
	}

	// close the pickedstock.txt
	o_file.close();
}

DWORD WINAPI StockPicker::threadProc(LPVOID lpParam)
{
	StockPicker* pSP = (StockPicker*)lpParam;

	pSP->m_isRunning  = true;

	pSP->pickup();

	pSP->m_isRunning = false;

	return 0;
}

void StockPicker::start(const char* stockfile)
{
	log("Load stock file: %s", stockfile);

	//
	m_stockProductData.load(stockfile);

	//
	m_hQuitHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

    //
	HANDLE handle = CreateThread(NULL, 0, StockPicker::threadProc, this, 0, NULL);

	if (handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
	}
}

void StockPicker::stop()
{
	SetEvent(m_hQuitHandle);
	
	while (m_isRunning)
	{
		Sleep(50);
	}

	CloseHandle(m_hQuitHandle);

	m_hQuitHandle = NULL;
}