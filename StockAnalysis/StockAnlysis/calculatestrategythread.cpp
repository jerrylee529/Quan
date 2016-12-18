#include "StdAfx.h"
#include "calculatestrategythread.h"
#include "Configuration.h"
#include "qa_msgno.h"
#include "crossMA_result_reader.h"
#include "Product.h"
#include "strategycalculator.h"
#include "instrumentmanager.h"
#include "weekline.h"
#include "MonthLine.h"
#include <boost/foreach.hpp>

//
bool CalculateStrategyThread::s_terminated_ = false;

ShareVector<CalculateStrategyThread*> CalculateStrategyThread::s_threads_;

ShareVector<StrategyResult> CalculateStrategyThread::s_strategyresults;

ShareQueue<Instrument> CalculateStrategyThread::s_queue_instrument_;

CrossMAResultReader CalculateStrategyThread::reader_;

vector<string> CalculateStrategyThread::strategies_;

CalculateStrategyThread::CalculateStrategyThread(HWND hwnd_progress, ShareQueue<Instrument>* instruments,
	const DataHandler& datahandler, int cycle, CrossMAResultReader* reader)
: hwnd_progress_(hwnd_progress), instruments_(instruments), 
datahandler_(datahandler), cycle_(cycle)
{

}

CalculateStrategyThread::~CalculateStrategyThread(void)
{

}

DWORD CalculateStrategyThread::CalculateStrategyProc(void* lpParam)
{
	CalculateStrategyThread* ccMA = (CalculateStrategyThread*)lpParam;

	// 线程队列增加一个线程
	s_threads_.AddItem(ccMA);

	HWND hwnd = ccMA->hwnd_progress_;

	while(!ccMA->instruments_->empty() && !CalculateStrategyThread::s_terminated_)
	{
		// 得到一个品种代码
		Instrument instrument = ccMA->instruments_->Popup();

		// 发送进度信息
		if (hwnd != 0)
		{
			MsgProgressSetPos* msg_pgr_set_pos = new MsgProgressSetPos;
			msg_pgr_set_pos->pos = 0;

			stringstream ss;

			ss<<"开始计算"<<instrument.Code<<","<<instrument.Name;

			msg_pgr_set_pos->prompt = ss.str();

			PostMessage(hwnd, QAM_PROGRESS_SETPOS, (WPARAM)msg_pgr_set_pos, 0);
		}

		// 读取品种历史行情
		string path = g_Configuration.getHitoryDataStorePath();
		path += instrument.Code;
		path += ".txt";

		Product product(instrument.Code.c_str(), instrument.Name.c_str());

		cout<<"读取文件"<<path<<endl;

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
		if (hwnd != 0)
		{
			MsgProgressDone* msg_pgr_done = new MsgProgressDone;
			msg_pgr_done->prompt = "计算完成";

			PostMessage(hwnd, QAM_PROGRESS_DONE, (WPARAM)msg_pgr_done, 0);
		}

		s_terminated_ = true;
	}

	return 0;
}

void CalculateStrategyThread::Execute()
{
	HANDLE hThread = CreateThread(NULL, 0, CalculateStrategyProc, this, 0, 0);

	if (hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hThread);
	}
}

void CalculateStrategyThread::Terminate()
{
	s_terminated_ = true;

	while(s_threads_.GetCount() > 0)
	{
		Sleep(50);
	}

	s_terminated_ = false;
}

void CalculateStrategyThread::ExecuteStrategy(Product* product)
{
	StrategyCalculator sc;

	foreach(strategies_, it, vector<string>)
	{
		sc.AddStrategy((*it).c_str(), datahandler_);
	}

	vector<Quot> quots;

	switch (cycle_)
	{
	case 0:
		quots = product->GetQuotations();
		break;
	case 1: 
		{
			WeekLine wl(&quots);

			foreach(product->GetQuotations(), it, vector<Quot>)
			{
				wl.Calculate(*it);
			}
		}
		break;
	case 2:
		{
			MonthLine ml(&quots);

			foreach(product->GetQuotations(), it, vector<Quot>)
			{
				ml.Calculate(*it);
			}
		}
		break;
	default:
		break;
	}

	sc.Calculate(quots);

	StrategyResult res = sc.result();
	res.code_ = product->code();
	s_strategyresults.AddItem(res);
}

void CalculateStrategyThread::SetStrategy(const vector<string>& strategies)
{
	strategies_.clear();

	strategies_.assign(strategies.begin(), strategies.end());
}

void CalculateStrategyThread::Run(int sector, HWND hwnd_progress, const DataHandler& datahandler, int cycle)
{
	//
	cout<<"清空代码队列"<<endl;

	s_queue_instrument_.clear();

	//
	cout<<"清空策略结果"<<endl;
	s_strategyresults.Clear();

	vector<Instrument>& vtInstrument = g_InstrumentMgr.GetInstruments((InstrumentManager::SECTOR)sector);

	foreach(vtInstrument, it, vector<Instrument>)
	{
		s_queue_instrument_.Push(*it);
	}

	if (hwnd_progress != 0)
	{
		MsgProgressSetRange* msg_pgr_setrange = new MsgProgressSetRange;
		msg_pgr_setrange->minimize = 0;
		msg_pgr_setrange->maximize = vtInstrument.size();
		msg_pgr_setrange->title = "开始计算策略";

		PostMessage(hwnd_progress, QAM_PROGRESS_SETRANGE, (WPARAM)msg_pgr_setrange, 0);
	}

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	cout<<"获取系统CPU数量"<<si.dwNumberOfProcessors<<endl;

	for (size_t i=1; i<=si.dwNumberOfProcessors; i++)
	{
		CalculateStrategyThread* ccma = new CalculateStrategyThread(hwnd_progress, 
			&s_queue_instrument_, datahandler, cycle, &reader_);

		ccma->Execute();
	}
}

void CalculateStrategyThread::Run(const vector<Instrument>& instruments, HWND hwnd_progress, const DataHandler& datahandler, int cycle)
{
	//
	s_terminated_ = false;

	//
	s_queue_instrument_.clear();

	//
	s_strategyresults.Clear();

	//
	foreach(instruments, it, vector<Instrument>)
	{
		s_queue_instrument_.Push(*it);
	}

	MsgProgressSetRange* msg_pgr_setrange = new MsgProgressSetRange;
	msg_pgr_setrange->minimize = 0;
	msg_pgr_setrange->maximize = instruments.size();
	msg_pgr_setrange->title = "开始计算策略";

	PostMessage(hwnd_progress, QAM_PROGRESS_SETRANGE, (WPARAM)msg_pgr_setrange, 0);

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	for (size_t i=1; i<max(si.dwNumberOfProcessors, 2); i++)
	{
		CalculateStrategyThread* ccma = new CalculateStrategyThread(hwnd_progress, 
			&s_queue_instrument_, datahandler, cycle, &reader_);

		ccma->Execute();
	}
}

bool CalculateStrategyThread::getTerminated()
{
	return s_terminated_;
}

//
ThreadExecutor::ThreadExecutor(const vector<Instrument>& instruments, HWND hwnd_progress, const DataHandler& datahandler, int cycle)
	: hwnd_progress_(hwnd_progress), datahandler_(datahandler), cycle_(cycle), terminated_(true)
{
	BOOST_FOREACH(Instrument item, instruments)
	{
		instruments_.Push(item);
	}
}

ThreadExecutor::~ThreadExecutor()
{

}

void ThreadExecutor::start()
{
	terminated_ = false;

	int nThreadNum = datahandler_.GetItem<int>("thread_num", 3);

	for(int num=0;num<nThreadNum;num++)
		group_.create_thread(boost::bind(&ThreadExecutor::executeThread, this));

	if (hwnd_progress_ != 0)
	{
		MsgProgressDone* msg_pgr_done = new MsgProgressDone;
		msg_pgr_done->prompt = "计算完成";

		PostMessage(hwnd_progress_, QAM_PROGRESS_DONE, (WPARAM)msg_pgr_done, 0);
	}

	terminated_ = true;
}

void ThreadExecutor::stop()
{
	terminated_ = true;

	group_.join_all();
}

void ThreadExecutor::executeThread()
{
	while(!terminated_)
	{
		if (instruments_.empty())
		{
			break;
		}

		Instrument item = instruments_.Popup();

		//
		if (item.Code.empty())
		{
			continue;
		}

		// 发送进度信息
		if (hwnd_progress_ != 0)
		{
			MsgProgressSetPos* msg_pgr_set_pos = new MsgProgressSetPos;
			msg_pgr_set_pos->pos = 0;

			stringstream ss;

			ss<<"开始计算"<<item.Code<<","<<item.Name;

			msg_pgr_set_pos->prompt = ss.str();

			PostMessage(hwnd_progress_, QAM_PROGRESS_SETPOS, (WPARAM)msg_pgr_set_pos, 0);
		}

		LOG_DEBUG<<"开始计算"<<item.Code<<", "<<item.Name<<LOG_ENDL;

		// 读取品种历史行情
		string path = g_Configuration.getHitoryDataStorePath();
		path += item.Code;
		path += ".csv";

		Product product(item.Code.c_str(), item.Name.c_str());

		LOG_DEBUG<<"读取文件"<<path<<LOG_ENDL;

		product.Load();

		// 执行策略计算
		try
		{
			vector<Quot> quots;

			switch (cycle_)
			{
			case 0:
				quots = product.GetQuotations();
				break;
			case 1: 
				{
					WeekLine wl(&quots);

					BOOST_FOREACH(Quot& quot, product.GetQuotations())
					{
						wl.Calculate(quot);
					}
				}
				break;
			case 2:
				{
					MonthLine ml(&quots);

					BOOST_FOREACH(Quot& quot, product.GetQuotations())
					{
						ml.Calculate(quot);
					}
				}
				break;
			default:
				break;
			}

			// 执行
			doExecute(item.Code.c_str(), quots);
		}
		catch (...)
		{
			continue;
		}

		Sleep(100);
	}

}

//
StrategyExecutor::StrategyExecutor(const vector<Instrument>& instruments, const vector<string>& strategies, HWND hwnd_progress, const DataHandler& datahandler, int cycle)
	: strategies_(strategies), hwnd_progress_(hwnd_progress), datahandler_(datahandler), cycle_(cycle), terminated_(true)
{
	BOOST_FOREACH(Instrument item, instruments)
	{
		instruments_.Push(item);
	}
}

StrategyExecutor::~StrategyExecutor()
{

}

void StrategyExecutor::start()
{
	boost::thread_group group;

	terminated_ = false;

	int nThreadNum = datahandler_.GetItem<int>("thread_num", 3);

	for(int num=0;num<nThreadNum;num++)
		group.create_thread(boost::bind(&StrategyExecutor::executeThread, this));

	group.join_all();

	if (hwnd_progress_ != 0)
	{
		MsgProgressDone* msg_pgr_done = new MsgProgressDone;
		msg_pgr_done->prompt = "计算完成";

		PostMessage(hwnd_progress_, QAM_PROGRESS_DONE, (WPARAM)msg_pgr_done, 0);
	}

	terminated_ = true;
}

void StrategyExecutor::stop()
{
	terminated_ = true;
}

void StrategyExecutor::executeThread()
{
	while(!terminated_)
	{
		if (instruments_.empty())
		{
			break;
		}

		Instrument item = instruments_.Popup();

		//
		if (item.Code.empty())
		{
			continue;
		}

		// 发送进度信息
		if (hwnd_progress_ != 0)
		{
			MsgProgressSetPos* msg_pgr_set_pos = new MsgProgressSetPos;
			msg_pgr_set_pos->pos = 0;

			stringstream ss;

			ss<<"开始计算"<<item.Code<<","<<item.Name;

			msg_pgr_set_pos->prompt = ss.str();

			PostMessage(hwnd_progress_, QAM_PROGRESS_SETPOS, (WPARAM)msg_pgr_set_pos, 0);
		}

		LOG_DEBUG<<"开始计算"<<item.Code<<", "<<item.Name<<LOG_ENDL;

		// 读取品种历史行情
		string path = g_Configuration.getHitoryDataStorePath();
		path += item.Code;
		path += ".txt";

		Product product(item.Code.c_str(), item.Name.c_str());

		LOG_DEBUG<<"读取文件"<<path<<LOG_ENDL;

		product.Load();

		// 执行策略计算
		try
		{
			StrategyCalculator sc;

			foreach(strategies_, it, vector<string>)
			{
				sc.AddStrategy((*it).c_str(), datahandler_);
			}

			vector<Quot> quots;

			switch (cycle_)
			{
			case 0:
				quots = product.GetQuotations();
				break;
			case 1: 
				{
					WeekLine wl(&quots);

					BOOST_FOREACH(Quot& quot, product.GetQuotations())
					{
						wl.Calculate(quot);
					}
				}
				break;
			case 2:
				{
					MonthLine ml(&quots);

					BOOST_FOREACH(Quot& quot, product.GetQuotations())
					{
						ml.Calculate(quot);
					}
				}
				break;
			default:
				break;
			}

			sc.Calculate(quots);

			StrategyResult res = sc.result();

			res.code_ = product.code();

			strategyresults_.AddItem(res);		
		}
		catch (...)
		{
			continue;
		}

		Sleep(100);
	}

}

ShareVector<StrategyResult>& StrategyExecutor::getStrategyResult()
{
	return strategyresults_;
}