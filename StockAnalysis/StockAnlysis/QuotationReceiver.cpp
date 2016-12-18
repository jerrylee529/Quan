#include "StdAfx.h"
#include "QuotationReceiver.h"
#include "HttpDownloader.h"
#include <WinInet.h>
#include "Utilities.h"

// 联美控股,23.00,21.95,22.16,23.00,21.56,22.17,22.18,6131849,136495989,2600,22.17,7200,22.16,16600,22.15,8100,22.14,2900,22.10,15000,22.18,8300,22.19,8500,22.20,4500,22.24,1193,22.25,2015-05-22,15:04:07,00
// 0: 名称
// 1: 开盘价
// 2: 昨收
// 3: 当前价
// 4: 今最高
// 5: 今最低
// 6: 买入价
// 7: 卖出价
// 8: 成交量
// 9: 成交额
//10: 买一

#define MAXSIZE 4096

#pragma comment(lib, "Wininet.lib")


QuotationReceiver::QuotationReceiver()
: eventlistener_(0), terminated_(true)
{
}

QuotationReceiver::~QuotationReceiver()
{
}

void QuotationReceiver::start(QuotationEventListener* eventlistener, DataHandler& dataHandler)
{
	terminated_ = false;

	int nThreadNum = dataHandler.GetItem<int>("quot_thread_num", 3);

	for(int num=0;num<nThreadNum;num++)
		thread_group_.create_thread(boost::bind(&QuotationReceiver::receiveProc, this));
}

void QuotationReceiver::stop()
{
	terminated_ = true;

	thread_group_.join_all();
}

void QuotationReceiver::receiveProc()
{
	while(!terminated_)
	{
		Instrument instrument = instruments_.Popup();

		string strUrl = "http://hq.sinajs.cn/list=";

		strUrl += instrument.getFullCode();

		string str = OpenUrl(strUrl.c_str());

		size_t nPos = 0;

		map<string, Quot> quotations;

		string strCode;

		string strToken = GetSubBtFind(str.c_str(), strCode);

		char szpstr[1024];
		strcpy(szpstr, strToken.c_str());

		vector<string> vec;
		Token(",", szpstr, vec);

		if (vec.size() > 8)
		{
			Quot quot;
			quot.setDatetime(Date::GetDateOfToday().c_str());
			quot.Open = convert<string, double>(vec[1])*100;
			quot.LastClose = convert<string, double>(vec[2])*100;
			quot.Close = convert<string, double>(vec[3])*100;
			quot.High = convert<string, double>(vec[4])*100;
			quot.Low = convert<string, double>(vec[5])*100;
			quot.Volume = convert<string, int>(vec[8]);
			quotations.insert(make_pair(strCode, quot));
		}

		// 更新行情
		if (eventlistener_ && !quotations.empty())
		{
			eventlistener_->DoOnRecv(quotations);
		}

		// 5秒更新一次避免UI被占用
		Sleep(500);
	}
}


string QuotationReceiver::OpenUrl(const char* url)
{
	string strRet("");

	HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if(hSession != NULL)
	{
		HINTERNET hHttp = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (hHttp != NULL)
		{
			char Temp[MAXSIZE];

			ULONG Number = 1;

			while (Number > 0)
			{
				memset(Temp, 0, sizeof(Temp));

				InternetReadFile(hHttp, Temp, MAXSIZE - 1, &Number);

				strRet += Temp;
			}

			InternetCloseHandle(hHttp);
		
			hHttp = NULL;
		}

		InternetCloseHandle(hSession);

		hSession = NULL;
	}

	return strRet;
}

string QuotationReceiver::GetSubBtFind(const char* lpsz, string& strCode)
{
	string scrStr = lpsz;

	string str_temp = "";

	int len = strlen(lpsz);

	int nPos = scrStr.find_last_of('_');

	if (nPos == string::npos)
	{
		return "";
	}

	int nPosOfComma = scrStr.find_first_of('=');

	if (nPosOfComma == string::npos)
	{
		return "";
	}

	// 取得股票代码
	strCode = scrStr.substr(nPos+1+2, nPosOfComma-nPos-1-2);

	int beginPos = scrStr.find("\"");

	scrStr = scrStr.substr(beginPos+1,len);

	int endPos = scrStr.find("\"");

	str_temp = scrStr.substr(0,endPos);

	return str_temp;

}

int QuotationReceiver::Token(const char* pSep, char* pStr, vector<string>& refvec)
{
	for(char* outer = strtok(pStr, pSep); NULL != outer; outer = strtok(NULL, pSep))
	{
		refvec.push_back(outer);
	}

	return 0;
}

bool QuotationReceiver::IsRunning()
{
	return !terminated_;
}
