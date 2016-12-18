#pragma once

/********************************************************************
	created:	2015/05/23
	created:	23:5:2015   0:16
	filename: 	e:\Quan\StockAnlysis\StockAnlysis\QuotationReceiver.h
	file path:	e:\Quan\StockAnlysis\StockAnlysis
	file base:	QuotationReceiver
	file ext:	h
	author:		Jerry Li
	
	purpose:	行情接收类，负责从新浪接收行情并发给相应的品种
*********************************************************************/

#include <string>
#include <vector>
#include "StockDataStructures.h"
#include "FrameworkCls.h"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace fmkcls;

//
class QuotationEventListener
{
public:
	virtual void DoOnRecv(const map<string, Quot>& quotations) = 0;
};


//
class QuotationReceiver
{
public:
	QuotationReceiver();

	~QuotationReceiver(void);
public:
	// 开始接收
	void start(QuotationEventListener* eventlistener, DataHandler& dataHandler);

	// 是否运行
	bool IsRunning();

	// 终止所有的行情线程
	void stop();

private:
	void receiveProc();

	AccessLock s_lock_;

private:
	string OpenUrl(const char* url);

	string GetSubBtFind(const char* lpsz, string& strCode);

	int Token(const char* pSep, char* pStr, vector<string>& refvec);

	QuotationEventListener* eventlistener_;

	boost::thread_group thread_group_;

	bool terminated_;

	// 需要获取行情的品种列表
	ShareQueue<Instrument> instruments_;
};
