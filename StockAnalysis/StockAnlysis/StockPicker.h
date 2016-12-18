#ifndef _STOCK_PICKER
#define _STOCK_PICKER

#include <vector>
#include "Utilities.h"
#include "StockProductData.h"

using namespace std;

class StockPicker
{
public:
	StockPicker(void);
public:
	~StockPicker(void);

	void start(const char* stockfile);

	void stop();

	bool isRunning()
	{
		return m_isRunning;
	}

	void setProgress(Progress* progress)
	{
		m_progess = progress;
	}

private:
	void pickup();

	static DWORD WINAPI threadProc(LPVOID lpParam);

	StockProductData m_stockProductData;

	vector<StockProduct> m_pickedStockProducts;
	
	int m_cycle;
	int m_downloadInterval;
	bool m_isRunning;
	HANDLE m_hQuitHandle;
	Progress* m_progess;

};

#endif
