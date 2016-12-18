#ifndef _STOCK_DATA_STRUCTURE_
#define _STOCK_DATA_STRUCTURE_

#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

// 无效值定义
const int INVALID_VALUE = -9999;

// Stock information
struct Instrument
{
	string Code;   // 600001
    string Market; // SZ or SS
    string Name;   // GOOG
	float pe;     // 市盈率
	float pb;     // 市净率
	float esp;    // 每股收益
	float bvps;   // 每股净资产
	
	Instrument()
	{
	}

	int getCodeByInt() const
	{
		return atoi(Code.c_str());
	}

	// 得到完整的股票代码, Market+Code
	string getFullCode() const
	{
		string str(Market);

		transform(str.begin(), str.end(), str.begin(), tolower);

		str += Code;

		return str;
	}

	// 板块，返回值：0: 上海主板，1: 深圳主板，2: 中小板， 3: 创业板
	unsigned char getSector() const
	{
		string str;
		str.append(Code, 0, 3);

		unsigned char ret = 0;

		if (str == "600")
		{
			ret = 0;
		}
		else if (str == "000")
		{
			ret = 1;
		}
		else if (str == "002")
		{
			ret = 3;
		}
		else 
		{
			ret = 4;
		}
	}
};


// Stock price and volume information of each day
struct Quot
{
	char Datetime[32];
	int Open;     // need to multiply 100
	int High;     // need to multiply 100
	int Low;      // need to multiply 100
	int Close;    // need to multiply 100
	double Volume;
	int AdjClose; // need to multiply 100
	int LastClose; // 昨收

	Quot()
	{
		memset(Datetime, 0, sizeof(Datetime));

		Open = 0;
		High = 0;
		Low = 0;
		Close = 0;
		Volume = 0;
		AdjClose = 0;
	}

	double getOpen() const
	{
		return (double)Open/100;
	}

	double getClose() const
	{
		return (double)Close/100;
	}

	double getHigh() const
	{
		return (double)High/100;
	}

	double getLow() const
	{
		return (double)Low/100;
	}

	double getVolume() const // 除掉10000以防溢出
	{
		return Volume;
	}

	void setDatetime(const char* value)
	{
		memcpy(Datetime, value, sizeof(Datetime));
	}

	string getDatetime() const
	{
		return string(Datetime);
	}

	string getDate() const
	{
		string ret;
		string strDatetime(Datetime);
		ret = strDatetime.substr(0, 10);

		return ret;
	}

	struct tm getDatetimeST() const
	{
		struct tm t;
        
		string str(Datetime);

		int year, month, day, hour, minute,second;  

		sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);  

		t.tm_year = year - 1900; 
		t.tm_mon = month - 1;
		t.tm_mday = day;
		t.tm_hour = hour;
		t.tm_min = minute;
		t.tm_sec = second;

		return t;
	}

	bool operator==(const Quot& value)
	{
		string date1(Datetime);
	    string date2(value.Datetime);

		return (date1==date2)?true:false;
	}

	bool isUpLine() const
	{
		return (Close > Open)?true:false;
	}

	bool isDownLine() const
	{
		return (Close < Open)?true:false;
	}

	bool isEvenLine() const
	{
		return (Close == Open)?true:false;
	}

	// 得到涨跌幅
	double GetChange() const
	{
		if (LastClose == INVALID_VALUE)
		{
			return 0;
		}
		else
		{
			return (double)(Close - LastClose)*100/LastClose;
		}
	}
};

// 异常
class QAException 
{
public:
	QAException(const int errorcode, const string& message)
		:ErrorCode(errorcode), Message(message)
	{
		
	}

	int ErrorCode;

	string Message;
};


#endif