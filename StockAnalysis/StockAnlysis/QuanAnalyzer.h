#pragma once

#include "Utilities.h"
#include "StockDataStructures.h"
#include "Product.h"
#include "FrameworkCls.h"
#include "crossMA_result_reader.h"
#include "QuotationReceiver.h"
#include "qa_msgno.h"

using namespace fmkcls;


//
class QuanAnalyzer : public Singleton<QuanAnalyzer>, public QuotationEventListener
{
	friend class Singleton<QuanAnalyzer>;
public:
	// 初始化
	void Init(HWND hwnd);

	// 获取一个品种对象
	Product* GetProduct(const char* code);

	// 删除一个品种
	void RemoveProduct(const char* code);

	// 为一个品种添加策略
	// code: 品种代码
	// strategyName: 策略名称
	Product* AddStrategy(const char* code, const char* strategyName);
	
	// 开始获取行情
	void StartQuotReceiver();

	// 停止接收行情
	void StopQuotReceiver();

	//
	bool IsQuotReceiverRunning();

	//
	CrossMAResultItem GetCrossMAResultItem(const char* code);

	//
	void Load(const char* filename);

	//
	void Save(const char* filename);

	// 更新行情
	virtual void DoOnRecv(const map<string, Quot>& quotations);

protected:
	QuanAnalyzer(void);

	~QuanAnalyzer(void);

private:
	// 载入一个品种
	Product* LoadProduct(const char* code, const char* filename);

	typedef vector<Instrument> VTINSTRUMENT;
	typedef map<string, Product*> MAPPRODUCT; 

	MAPPRODUCT mapProduct; // 股票对象列表

	VTINSTRUMENT vtInstrument[7]; // 股票代码列表

	FileLoggerWriter fileLogWriter;

	HWND hwnd_progress_;

	ShareQueue<Instrument>* queue_instrument_;

	// 读取已经写入文件的品种
	CrossMAResultReader crossma_reader_;

	// 行情接收器
	QuotationReceiver* quotation_receiver_;

	//
	HWND hwnd_;
};

extern QuanAnalyzer& g_QuanAnalyer;