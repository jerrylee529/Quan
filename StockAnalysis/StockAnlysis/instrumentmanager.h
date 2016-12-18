#pragma once

#include "Utilities.h"
#include "StockDataStructures.h"

class InstrumentManager : public Singleton<InstrumentManager>
{
	friend class Singleton<InstrumentManager>;
public:
	enum SECTOR { SH = 0, SZ, CYB, ZXB, ZXBK, INDEX, ALL};

	SECTOR GetCurrentSector();

	void SetCurrentSector(SECTOR sector);

	vector<Instrument>& GetInstruments(SECTOR sector);

	Instrument GetInstrument(const char* code);

protected:
	InstrumentManager(void);
	~InstrumentManager(void);

private:
	void Load(const char* strFileName, vector<Instrument>& vtInstrument);

	typedef vector<Instrument> VTINSTRUMENT;

	VTINSTRUMENT vtInstrument[7]; // 股票代码列表

	SECTOR sector_;
};

extern InstrumentManager& g_InstrumentMgr;