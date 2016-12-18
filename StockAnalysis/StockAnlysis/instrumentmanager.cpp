#include "StdAfx.h"
#include "instrumentmanager.h"
#include "Configuration.h"


InstrumentManager& g_InstrumentMgr = InstrumentManager::getInstance();

InstrumentManager::InstrumentManager(void)
: sector_(SH)
{
}

InstrumentManager::~InstrumentManager(void)
{
}

vector<Instrument>& InstrumentManager::GetInstruments(SECTOR sector)
{
	const string INSTRUMENT_FILE[] = {"SH_Stock.txt", "SZ_Stock.txt", "CYB_Stock.txt", "ZXB_Stock.txt", "ZXBK_Stock.txt", "Indexs.txt", "instruments.csv"};

	vector<Instrument>& instruments = vtInstrument[sector];

	if (instruments.empty())
	{
		string strStockProductFile = g_Configuration.GetInstrumentFilePath();

		strStockProductFile += INSTRUMENT_FILE[sector];

		Load(strStockProductFile.c_str(), instruments);
	}

	return instruments;	
}

void InstrumentManager::Load(const char* strFileName, vector<Instrument>& vtInstrument)
{
	string line, field;    //line为每行内容，field为每个字段

	cout << "读取代码文件: " << strFileName << endl;

	//打开文件
	ifstream in;
	in.open(strFileName);    //以逗号隔开

	//处理文件内容
	if(in.is_open())
	{
		// remove the first row because it's column headers
		getline(in, line);

		//循环取出每一行数据
		while(getline(in,line))
		{
			istringstream stream(line);

			//将一行的多个字段取出
			vector<string> RowContext;

			//以‘,’读取每个字段
			while(getline(stream, field, '\t'))
			{
				//存入
				RowContext.push_back(field);
			}

			if (RowContext.size() < 2)
			{
				continue;
			}

			Instrument instrument;

			string strMarket;
			strMarket.append(RowContext[0], 0, 2);

			string strCode;
			strCode.append(RowContext[0], 2, RowContext[0].length());

			instrument.Code = RowContext[0];

			instrument.Market = (instrument.Code[0] == '6')?"SH":"SZ";

			instrument.Name = RowContext[1];

			vtInstrument.push_back(instrument);
		}

		// 关闭文件
		in.close();
	}
	else
	{
		//文件打开失败
		cout << "打开文件失败！";
	}
}

InstrumentManager::SECTOR InstrumentManager::GetCurrentSector()
{
	return sector_;
}

void InstrumentManager::SetCurrentSector(SECTOR sector)
{
	sector_ = sector;
}

Instrument InstrumentManager::GetInstrument(const char* code)
{
	string str = "";

	vector<Instrument>& instruments = GetInstruments(ALL);

	Instrument instr;

	foreach(instruments, it, vector<Instrument>)
	{
		if (it->Code == string(code))
		{
			instr = *it;

			break;
		}
	}

	return instr;
}