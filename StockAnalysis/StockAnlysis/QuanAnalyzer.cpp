#include "StdAfx.h"
#include "QuanAnalyzer.h"
#include "Configuration.h"
#include "qa_msgno.h"
#include "crossMA_result_reader.h"
#include "CalculateCrossMA.h"
#include "CrossMAStrategy.h"
#include "TrendStrategy.h"
#include "CalculateCrossMACD.h"
#include "VolumeCrossStrategy.h"
#include "TrendMACrossStrategy.h"
#include "instrumentmanager.h"

QuanAnalyzer& g_QuanAnalyer = QuanAnalyzer::getInstance();

QuanAnalyzer::QuanAnalyzer(void)
:queue_instrument_(0), quotation_receiver_(0)
{
}

QuanAnalyzer::~QuanAnalyzer(void)
{
	foreach(mapProduct, it, MAPPRODUCT)
	{
		delete it->second;
	}

	mapProduct.clear();

	if (queue_instrument_)
	{
		delete queue_instrument_;
	}
}

void QuanAnalyzer::Init(HWND hwnd)
{
	hwnd_ = hwnd;

	// 设置日志输出
	Logger::getInstance().setLoggerWriter(&fileLogWriter);

	// 载入均线交叉策略的数据
	crossma_reader_.Load("E://Stock//crossma.tsv");
}

Product* QuanAnalyzer::LoadProduct(const char* code, const char* filename)
{
	Product* product = 0;

	MAPPRODUCT::iterator it = mapProduct.find(code);

	if (it == mapProduct.end())
	{
		product = new Product(code);

		product->Load();

		mapProduct.insert(make_pair(code, product));
	}
	else
	{
		product = it->second;
	}

	return product;
}

Product* QuanAnalyzer::GetProduct(const char* code)
{
	string path = g_Configuration.getHitoryDataStorePath();
	path += code;
	path += ".txt";

	Product* pRet = 0;

	MAPPRODUCT::iterator it = mapProduct.find(code);

	// 如果找到则直接返回
	if (it != mapProduct.end())
	{
		pRet = it->second;
	}
	else // 找不到则重新初始化该品种
	{
		pRet = LoadProduct(code, path.c_str());
	}

	return pRet;
}

void QuanAnalyzer::RemoveProduct(const char* code)
{
	MAPPRODUCT::iterator it = mapProduct.find(code);

	if (it != mapProduct.end())
	{
		Product* pRet = it->second;

		mapProduct.erase(it);

		delete pRet;
	}
}

Product* QuanAnalyzer::AddStrategy(const char* code, const char* strategyName)
{
	Product* product = GetProduct(code);

	if (product)
	{
		DataHandler dh;
	}

	return product;
}

void QuanAnalyzer::StartQuotReceiver()
{
	vector<Instrument>& instruments = g_InstrumentMgr.GetInstruments(InstrumentManager::ZXBK);

	QuotationReceiver* quotation_receiver = new QuotationReceiver();

	quotation_receiver->start(this, g_Configuration.getDataHandler());
}

void QuanAnalyzer::StopQuotReceiver()
{

}

bool QuanAnalyzer::IsQuotReceiverRunning()
{
	return false;
}

void QuanAnalyzer::DoOnRecv(const map<string, Quot>& quotations)
{
	MsgRecvQuotation* recv_quot = new MsgRecvQuotation;

	typedef map<string, Quot> MAPQUOT;

	foreach(quotations, it, MAPQUOT)
	{
		Product* product = GetProduct(it->first.c_str());

		/*
		if (product)
		{
			product->UpdateLatestQuot(it->second);
		}
		*/

		recv_quot->quotations.insert(make_pair(it->first, it->second));
	}

	PostMessage(hwnd_, QAM_QUOTATION_LATEST, (WPARAM)recv_quot, 0);
}

CrossMAResultItem QuanAnalyzer::GetCrossMAResultItem(const char* code)
{
	return crossma_reader_.GetItem(code);
}

void QuanAnalyzer::Save(const char* filename)
{
	crossma_reader_.Save(filename);
}

void QuanAnalyzer::Load(const char* filename)
{
	crossma_reader_.Clear();

	crossma_reader_.Load(filename);
}