// StockAnalysisService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "StockAnalysisService.h"
#include "Utilities.h"
#include "Configuration.h"
#include "QuanAnalyzer.h"
#include "calculatestrategythread.h"
#include "instrumentmanager.h"
#include "statresultfile.h"
#include <boost/program_options.hpp>

using namespace boost::program_options;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;


bool ma_sortfunc(StrategyResult p1, StrategyResult p2)
{
	return p1.GetRateOfPosGain() > p2.GetRateOfPosGain();
}

void saveStrategyResult(fmkcls::ShareVector<StrategyResult>& strategyResult)
{
	vector<StrategyResult> vtSR;

	for (int i = 0; i < strategyResult.GetCount(); i++)
	{
		vtSR.push_back(strategyResult.GetItem(i));
	}

	int count = 0;
	double percent = 0;

	foreach(vtSR, it, vector<StrategyResult>)
	{
		percent += it->GetRateOfPosGain();

		count++;
	}

	string strDate = Date::GetDateOfToday();

	// 保存到文件
	CString strFileName;
	strFileName.Format("%s\\strategyresult.csv", g_Configuration.GetSaveFilePath().c_str());
	StatResultFile srf;
	srf.Save(vtSR, strFileName);

	// backup
	strFileName.Format("%s\\strategyresult_%s.csv", g_Configuration.GetSaveFilePath().c_str(), strDate.c_str());
    srf.Save(vtSR, strFileName);

	// 买入top5
	vector<StrategyResult> vtTopBuy;

	// 卖出top5
	vector<StrategyResult> vtTopSell;

	// 持有top5
	vector<StrategyResult> vtTopHold;

	foreach(vtSR, it, vector<StrategyResult>)
	{
		if (it->CanBuy())
		{
			vtTopBuy.push_back(*it);
		}
		else if (it->CanSell())
		{
			vtTopBuy.push_back(*it);
		}
		else if (it->CanHold())
		{
			Date date = Date::Today();
			Date last_budy_date;
			last_budy_date.fromStringYYYYMMDD(it->GetLastBuyDate().c_str());

			int days = date - last_budy_date;

			if (days < 30)
			{
				vtTopHold.push_back(*it);
			}
		}
		else
		{

		}
	}

	// 保存买入前五
	if (!vtTopBuy.empty())
	{
		sort(vtTopBuy.begin(), vtTopBuy.end(), ma_sortfunc);
	}

	strFileName.Format("%s\\buy_top5.csv", g_Configuration.GetSaveFilePath().c_str());
	srf.Save(vtTopBuy, strFileName);

	// backup
	strFileName.Format("%s\\buy_top5_%s.csv", g_Configuration.GetSaveFilePath().c_str(), strDate.c_str());
	srf.Save(vtTopBuy, strFileName);

	// 保存卖出前五
	if (!vtTopSell.empty())
	{
		sort(vtTopSell.begin(), vtTopSell.end(), ma_sortfunc);
	}

	strFileName.Format("%s\\sell_top5.csv", g_Configuration.GetSaveFilePath().c_str());
	srf.Save(vtTopSell, strFileName);

	// backup
	strFileName.Format("%s\\sell_top5_%s.csv", g_Configuration.GetSaveFilePath().c_str(), strDate.c_str());
	srf.Save(vtTopSell, strFileName);

	// 保存持有前五
	if (!vtTopHold.empty())
	{
		sort(vtTopHold.begin(), vtTopHold.end(), ma_sortfunc);
	}

	strFileName.Format("%s\\hold_top5.csv", g_Configuration.GetSaveFilePath().c_str());
	srf.Save(vtTopHold, strFileName);

	// backup
	strFileName.Format("%s\\hold_top5_%s.csv", g_Configuration.GetSaveFilePath().c_str(), strDate.c_str());
	srf.Save(vtTopHold, strFileName);
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	options_description opts("test options");  

	opts.add_options()  
		("help,h","help info")  
		("configuration,c",value<string>(),"configuration filename");  

	variables_map vm;  
	try  
	{  
		store(parse_command_line(argc,argv,opts), vm); // 分析参数  
	}  
	catch(boost::program_options::error_with_no_option_name &ex)  
	{  
		cout<<ex.what()<<endl;  
	}  

	notify(vm); // 将解析的结果存储到外部变量  

	string strConfFile;

	if (vm.count("help"))  
	{  
		cout<<opts<<endl;  
		return -1;  
	}  

	if(vm.count("configuration"))  
	{  
		strConfFile = vm["configuration"].as<string>();

		cout<<vm["configuration"].as<string>()<<endl;  
	}  

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		// 载入配置
		cout<< "读取配置文件" << endl;

		g_Configuration.Load(strConfFile.c_str());

		const string STRATEGYNAMES[] =
		{
			"CrossMA",
			"CrossMACD",
			"MACD",
			"Trend",
			"TrendCrossMA",
			"VolumeCross",
			"MA",
			"NewHigh",
			"Rebound"
		};

		//
		vector<string> strategies;
		strategies.push_back(STRATEGYNAMES[2]);

		int cycle = 0;

		//
		DataHandler datahandler;

		cout<< "运行策略计算" << endl;

		vector<Instrument>& vtInstrument = g_InstrumentMgr.GetInstruments(InstrumentManager::ALL);

		StrategyExecutor se(vtInstrument, strategies, 0, g_Configuration.getDataHandler(), cycle);

		se.start();
       
		saveStrategyResult(se.getStrategyResult());

		cout<<"策略计算结束"<<endl;
	}

	return nRetCode;
}
