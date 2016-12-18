#include "StdAfx.h"
#include "CalculateCrossMA.h"
#include "QuanAnalyzer.h"
#include "Configuration.h"
#include "qa_msgno.h"
#include "crossMA_result_reader.h"
#include "Product.h"
#include "strategycalculator.h"

bool CalculateCrossMA::s_terminated_ = false;

ShareVector<CalculateCrossMA*> CalculateCrossMA::s_threads_;

CalculateCrossMA::CalculateCrossMA(HWND hwnd_progress, ShareQueue<Instrument>* instruments,
								   CrossMAResultReader* reader)
:hwnd_progress_(hwnd_progress), instruments_(instruments), 
reader_(reader)
{

}

CalculateCrossMA::~CalculateCrossMA(void)
{

}

DWORD CalculateCrossMA::CalculateStrategyProc(void* lpParam)
{
	CalculateCrossMA* ccMA = (CalculateCrossMA*)lpParam;

	// 线程队列增加一个线程
	s_threads_.AddItem(ccMA);

	while(!ccMA->instruments_->empty() && !CalculateCrossMA::s_terminated_)
	{
		// 得到一个品种代码
		Instrument instrument = ccMA->instruments_->Popup();

		// 发送进度信息
		MsgProgressSetPos* msg_pgr_set_pos = new MsgProgressSetPos;
		msg_pgr_set_pos->pos = 0;

		stringstream ss;

		ss<<"开始计算"<<instrument.Code<<","<<instrument.Name;

		msg_pgr_set_pos->prompt = ss.str();

		PostMessage(ccMA->hwnd_progress_, QAM_PROGRESS_SETPOS, (WPARAM)msg_pgr_set_pos, 0);

		// 读取品种历史行情
		string path = g_Configuration.getHitoryDataStorePath();
		path += instrument.Code;
		path += ".txt";

		Product product(instrument.Code.c_str(), instrument.Name.c_str());

		product.Load();

		// 执行策略计算
		try
		{
			ccMA->ExecuteStrategy(&product);
		}
		catch (...)
		{
			continue;
		}

		product.Save();

		Sleep(50);
	}

	// 释放对象
	delete ccMA;

	// 线程队列删除一个线程
	s_threads_.RemoveItem(ccMA);

	// 全部完成则发送信息
	if (s_threads_.GetCount() == 0)
	{
		MsgProgressDone* msg_pgr_done = new MsgProgressDone;
		msg_pgr_done->prompt = "计算完成";

		PostMessage(ccMA->hwnd_progress_, QAM_PROGRESS_DONE, (WPARAM)msg_pgr_done, 0);
	}

	return 0;
}

void CalculateCrossMA::ExecuteStrategy(Product* product)
{
	double roi = 0;
	int shortterm = 0, longterm = 0;
	double maxgain = 0, maxloss = 0;
	size_t numofposgain = 0, numofbuy = 0, numofsell = 0;

	DataHandler dh;

	StrategyCalculator sc;
	sc.AddStrategy("CrossMA", dh);

	for (int x = 3; x <= 10; x++)
	{
		for (int y = 10; y <= 60; y += 2)
		{
			dh.SetItem<int>("shortterm", x);
			dh.SetItem<int>("longterm", y);
				
			sc.ReCalculate(product->GetQuotations(), dh);

			//
			if (roi < sc.GetROI())
			{
				roi = sc.GetROI();

				shortterm = x;

				longterm = y;

				maxloss = sc.GetMaxLoss();

				maxgain = sc.GetMaxGain();

				numofposgain = sc.GetNumOfPosGain();

				numofbuy = sc.buyPoints().size();

				numofsell = sc.sellPoints().size();
			}

			Sleep(50);
		}
	}

	//
	CrossMAResultItem item;
	item.AddField(product->code());
	item.AddField(convert<int, string>(shortterm));
	item.AddField(convert<int, string>(longterm));
	item.AddField(convert<double, string>(product->GetIncreaseRate()));
	item.AddField(convert<double, string>(roi));
	item.AddField(convert<double, string>(maxgain));
	item.AddField(convert<double, string>(maxloss));
	item.AddField(convert<int, string>(numofposgain));
	item.AddField(convert<int, string>(numofbuy));
	item.AddField(convert<int, string>(numofsell));

	reader_->AddItem(item);
}

void CalculateCrossMA::Execute()
{
	HANDLE hThread = CreateThread(NULL, 0, CalculateStrategyProc, this, 0, 0);

	if (hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hThread);
	}
}

void CalculateCrossMA::Terminate()
{
	s_terminated_ = true;

	while(s_threads_.GetCount() > 0)
	{
		Sleep(50);
	}

	s_terminated_ = false;
}