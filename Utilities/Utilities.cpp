#include "StdAfx.h"
#include "Utilities.h"
#include <string>

using namespace std;

// Date class
Date::Date()
:year(0),month(0),day(0),weekday(0)
{

}

void Date::fromString(const char* value)
{
    string str = value;

	size_t nPosFirst = str.find('-');

	if (nPosFirst != string::npos)
	{
		year = atoi(str.substr(0, nPosFirst).c_str());
	}
	else
	{
		return;
	}

	size_t nPosSecond = str.find('-', nPosFirst+1);

	if (nPosSecond != string::npos)
	{
		month = atoi(str.substr(nPosFirst+1, nPosSecond-nPosFirst-1).c_str());
	}
	else
	{
		return;
	}

	day = atoi(str.substr(nPosSecond+1, str.length()-nPosSecond-1).c_str());

	CTime time(year, month, day, 0, 0, 0);

    int nWeekday = time.GetDayOfWeek(); //1 as sunday, 7 as saturday

	if (nWeekday == 7)
	{
		weekday = SUNDAY;
	}
	else
	{
		weekday = nWeekday - 1;
	}
}    

void Date::fromStringYYYYMMDD(const char* value)
{
	string str = value;

	if (str.length() < 10)
	{
		return;
	}

	year = atoi(str.substr(0, 4).c_str());
	month = atoi(str.substr(5, 2).c_str());
	day = atoi(str.substr(8, 2).c_str());

	CTime time(year, month, day, 0, 0, 0);

	int nWeekday = time.GetDayOfWeek(); //1 as sunday, 7 as saturday

	if (nWeekday == 7)
	{
		weekday = SUNDAY;
	}
	else
	{
		weekday = nWeekday - 1;
	}
}

void Date::fromStringYYYYMMDDHHNNSS(const char* value)
{
	string str = value;

	if (str.length() == 10)
	{
		year = atoi(str.substr(0, 4).c_str());
		month = atoi(str.substr(5, 2).c_str());
		day = atoi(str.substr(8, 2).c_str());

		hour = 0;
		minute = 0;
		second = 0;
	}
	else if (str.length() >= 19)
	{
		sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);  
	}
	else
	{
		return;
	}

	CTime time(year, month, day, hour, minute, second);

	int nWeekday = time.GetDayOfWeek(); //1 as sunday, 7 as saturday

	if (nWeekday == 7)
	{
		weekday = SUNDAY;
	}
	else
	{
		weekday = nWeekday - 1;
	}
}

int Date::operator-(const Date& value)
{
	CTime time1(year,month,day,0,0,0);
	CTime time2(value.year,value.month,value.day,0,0,0);
	
	CTimeSpan span = time1 - time2;

	return span.GetDays();
}

bool Date::operator==(const Date& value)
{
	CTime time1(year,month,day,0,0,0);
	CTime time2(value.year,value.month,value.day,0,0,0);

	return (time1==time2)?true:false;
}

bool Date::operator>(const Date& value)
{
	CTime time1(year,month,day,0,0,0);
	CTime time2(value.year,value.month,value.day,0,0,0);


	return (time1>time2)?true:false;
}

bool Date::operator>=(const Date& value)
{
	CTime time1(year,month,day,0,0,0);
	CTime time2(value.year,value.month,value.day,0,0,0);


	return (time1>=time2)?true:false;
}

bool Date::isInSameWeek(const Date& value)
{
	CTime time1(year,month,day,0,0,0);
	CTime time2(value.year,value.month,value.day,0,0,0);

    CTimeSpan span1(weekday-1, 0, 0, 0) ;
	CTimeSpan span2(value.weekday-1, 0, 0, 0) ;

	CTime monday1 = time1 - span1;
	CTime monday2 = time2 - span2;

    return (monday1 == monday2)?true:false;
}

bool Date::isInSameMonth(const Date& value)
{
	return (month == value.month)?true:false;
}

bool Date::isInSameDay(const Date& value)
{
	return (day == value.day)?true:false;
}

string Date::GetDateOfToday()
{
	struct tm *local;
	time_t t;
	t = time(0);
	local = localtime(&t);

	char buf[32] = {0};
	sprintf(buf, "%04d-%02d-%02d", local->tm_year+1900, local->tm_mon+1, local->tm_mday);

	return string(buf);
}

Date Date::Today()
{
	string str = GetDateOfToday();

	Date res;

	res.fromStringYYYYMMDD(str.c_str());

	return res;
}