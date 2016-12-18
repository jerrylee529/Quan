#include "StdAfx.h"
#include "crossMA_result_reader.h"
#include "Utilities.h"

// 从文件中读取的列数
const int COLUMNS = 10;


CrossMAResultReader::CrossMAResultReader(void)
{
}

CrossMAResultReader::~CrossMAResultReader(void)
{
}

void CrossMAResultReader::Load(const char* filename)
{
	TSVFileReader<CrossMAResultItem> reader;

	reader.Read(filename, items_, COLUMNS);
}

void CrossMAResultReader::Save(const char* filename)
{
	// 写入文件
	lock_.Lock();

	ofstream os(filename, ios::out);

	if (os)
	{
		foreach(items_, it, vector<CrossMAResultItem>)
		{
			stringstream ss;

			ss<<it->GetCode()<<"	"<<it->GetShortTerm()<<"	"<<it->GetLongTerm()<<"	"<<0.0<<"	"<<it->GetROI()
				<<"	"<<it->GetMaxGain()<<"	"<<it->GetMaxLoss()<<"	"<<it->GetNumOfPosGain()<<"	"<<it->GetNumOfBuy()<<"	"<<it->GetNumOfSell();

			os<<ss.str()<<endl;

			ss.clear();
		}

		os.close();
	}

	lock_.Unlock();
}

bool CrossMAResultReader::IsExisting(const char* code)
{
	foreach(items_, it, vector<CrossMAResultItem>)
	{
		if (string(code) == it->GetCode())
		{
			return true;
		}
	}

	return false;
}

CrossMAResultItem CrossMAResultReader::GetItem(const char* code)
{
	CrossMAResultItem item;

	foreach(items_, it, vector<CrossMAResultItem>)
	{
		if (string(code) == it->GetCode())
		{
			item = *it;

			break;
		}
	}

	return item;
}

void CrossMAResultReader::AddItem(const CrossMAResultItem& item)
{
	lock_.Lock();

	items_.push_back(item);

	lock_.Unlock();
}

void CrossMAResultReader::Clear()
{
	lock_.Lock();

	items_.clear();

	lock_.Unlock();
}