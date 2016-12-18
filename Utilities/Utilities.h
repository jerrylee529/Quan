#ifndef _UTILITIES_
#define _UTILITIES_

#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

//
/*
#define foreach(container,it,type) \
	for(type::iterator it = (container).begin();it!=(container).end();++it)*/

#define foreach(container,it,type) \
	for(type::const_iterator it = (container).begin();it!=(container).end();++it)

// Singleton pattern
template <typename T>
class Singleton
{
public:
    static T &getInstance()
    {
        Init();
        return *instance_;
    }

protected:
    static void Init()
    {
        if (instance_ == 0)
        {
            instance_ = new T;
            atexit(Destroy); //程序结束时调用注册的函数
        }
    }

    static void Destroy()
    {
        delete instance_;
    }

	Singleton(){}
	~Singleton(){}

    static T *instance_;
};

template <typename T>
T *Singleton<T>::instance_ = 0;

// Logger class
#define MAXLOGSIZE 20000
#define MAXLINSIZE 16000

// Logger writer
class LoggerWriter
{
public:
	virtual void write(const char* msg) = 0;
protected:
private:
};

//
class FileLoggerWriter : public LoggerWriter
{
public:
	FileLoggerWriter()
	{
		time_t timep; 
		time(&timep); 
		struct tm *p = localtime(&timep); 

		char buf[64] = {0};

		sprintf(buf, "QA_%04d%02d%02d%02d%02d%02d.log", p->tm_year+1900,
			p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

		ofFile.open(buf);
	}

	~FileLoggerWriter()
	{
		ofFile.close();
	}

	virtual void write(const char* msg)
	{
		ofFile<<msg<<endl;
	}
private:
	ofstream ofFile;
};




// Logger class
class Logger : public Singleton<Logger>
{
	friend class Singleton<Logger>;
public:
#ifdef WIN32
	Logger()
	{
		InitializeCriticalSection(&cs_log);
	}

	~Logger()
	{
		DeleteCriticalSection(&cs_log);
	}

	void Lock(CRITICAL_SECTION *l) {
		EnterCriticalSection(l);
	}

	void Unlock(CRITICAL_SECTION *l) {
		LeaveCriticalSection(l);
	}
#else
	void Lock(CRITICAL_SECTION *l) {
		pthread_mutex_lock(l);
	}
	
	void Unlock(CRITICAL_SECTION *l) {
		pthread_mutex_unlock(l);
	}
#endif

	void LogV(const char *pszFmt,va_list argp) 
	{
		struct tm *now;
		struct timeb tb;
		char logstr[MAXLINSIZE+1];
		char flog[MAXLOGSIZE] = {0};
		char datestr[16];
		char timestr[16];
		char mss[4];

		if (NULL == pszFmt|| 0 == pszFmt[0]) return;

		_vsnprintf(logstr,MAXLINSIZE,pszFmt,argp);

		ftime(&tb);

		now = localtime(&tb.time);

		sprintf(datestr,"%04d-%02d-%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday);

		sprintf(timestr,"%02d:%02d:%02d",now->tm_hour     ,now->tm_min  ,now->tm_sec );

		sprintf(mss,"%03d",tb.millitm);

		sprintf(flog,"[%s %s.%s] %s",datestr,timestr,mss,logstr);

		if (m_pLoggerWriter)
		{
			m_pLoggerWriter->write(flog);
		}
		else
		{
			 
		}
	}

	void Log(const char *pszFmt,...) 
	{
		va_list argp;

		Lock(&cs_log);
		
		va_start(argp,pszFmt);
		LogV(pszFmt,argp);
		va_end(argp);

		Unlock(&cs_log);
	}

	void setLoggerWriter(LoggerWriter* pWriter)
	{
		m_pLoggerWriter = pWriter;
	}

private:
	CRITICAL_SECTION cs_log;
	LoggerWriter* m_pLoggerWriter;
};


#define LOG(format, ...)  \
	do \
{\
	Logger::getInstance().Log(format, ##__VA_ARGS__); \
} while (0);

// Date class
class Date
{
public:
	Date();

	void fromString(const char* value);

	void fromStringYYYYMMDD(const char* value);

	void fromStringYYYYMMDDHHNNSS(const char* value);

	bool isInSameWeek(const Date& value);

	bool isInSameMonth(const Date& value);

	bool isInSameDay(const Date& value);

	static string GetDateOfToday();

	static Date Today();

	int operator-(const Date& value);

	bool operator>(const Date& value);

	bool operator>=(const Date& value);

	bool operator==(const Date& value);

	int year;
	int month;
	int day;
    int weekday;
	int hour;
	int minute;
	int second;

	enum WEEKDAY
	{
		MONDAY=1,
		TUESDAY=2,
		WEDNESDAY=3,
		THURSDAY=4,
		FRIDAY=5,
		SATURDAY=6,
		SUNDAY=7
	};
protected:
private:
};


//
class Observable;

class Observer
{
public:
	virtual void update(Observable* observable, void* arg) = 0;	
};

//
class Observable
{
public:
	void addObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	void removeObserver(Observer* observer)
	{
		vector<Observer*>::iterator it = find(observers.begin(), observers.end(), observer);

		if (it != observers.end())
		{
			observers.erase(it);
		}
	}

	void notifyObservers(void* arg)
	{
		foreach(observers, it, vector<Observer*>)
		{
			(*it)->update(this, arg);
		}
	}

protected:
	vector<Observer*> observers;
};

// 类型转换函数
template<class in_value,class out_type>
out_type convert(const in_value& t)
{
	stringstream stream;

	stream<<t;//向流中传值

	out_type result;//这里存储转换结果

	stream>>result;//向result中写入值

	return result;
}



// 消息类
class Msg
{
public:
	Msg(void* sender, int msgCode)
		:_msgCode(msgCode), _sender(sender)
	{

	}

	int getMsgCode() { return _msgCode; }

	void* getSender() { return _sender; }

	virtual Msg* clone() = 0;

protected:
	int _msgCode;
	void* _sender;
};


// 广播消息模板类
// 信息模板
template<class MSGBODY>
class Notification : public Msg
{
public:
	Notification(void* sender, int msgCode, const MSGBODY data)
		:Msg(sender, msgCode), _body(data)
	{
	}

	MSGBODY& getBody()
	{
		return _body;
	}

	Notification<MSGBODY>* clone()
	{
		Notification<MSGBODY>* msg = new Notification<MSGBODY>(_sender, _msgCode, _body);

		return msg;
	}

private:
	MSGBODY _body;
};

//
// 用于处理广播消息对象的模板类
template<class T, class PARAM>
class MsgReleaser
{
public:
	MsgReleaser(PARAM param) { _msg = (T*) param; }
	~MsgReleaser() { delete _msg; }

	T* operator()()
	{
		return _msg;
	}

	T* operator->()
	{
		return _msg;
	}

	T& operator*()
	{
		return *_msg;
	}
private:
	T* _msg;
};

//
#define sendNotification(msgcode,islast) \
	if (islast)						\
{								\
	Notification<string> msg(this, msgcode, string(""));			\
	notifyObservers(&msg);	\
}

// 发送通知
#define sendNotificationMsg(msgcode,MSGBODYTYPE,body,islast) \
	if (islast)						\
{								\
	Notification<MSGBODYTYPE> msg(this, msgcode, body);			\
	notifyObservers(&msg);		\
}

// 错误处理
#define handle_error(errcode,errmsg,outerr,lastmsg) \
	if ((errcode) != 0) {	\
	(lastmsg) = (errmsg); \
	Notification<string> msg(this, (outerr), string(""));			\
	notifyObservers(&msg);	\
	return; \
	}


//  
class RowInfo
{
public:
	void AddField(const string& field)
	{
		fields.push_back(field);
	}

	size_t GetCount()
	{
		return fields.size();
	}
protected:
	vector<string> fields;
};


// T 必须为RowInfo的子类
template <class T>
class TSVFileReader
{
public:
	void Read(const char* strFileName, vector<T>& vtItems, const size_t columns)
	{
		string line, field;    //line为每行内容，field为每个字段

		//打开文件
		ifstream in;
		in.open(strFileName);    //以逗号隔开

		//处理文件内容
		if(in.is_open())
		{
			//循环取出每一行数据
			while(getline(in,line))
			{
				istringstream stream(line);

				//将一行的多个字段取出
				T RowContext;

				//以‘,’读取每个字段
				while(getline(stream, field, '	'))
				{
					//存入
					RowContext.AddField(field);
				}

				if (RowContext.GetCount() < columns)
				{
					continue;
				}

				vtItems.push_back(RowContext);
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
};

// 数据处理
class DataHandler
{
public:
	void Load(const char* filename)
	{
		// 一行 
		string strRow;

		//打开文件
		ifstream in;

		in.open(filename);    //以逗号隔开

		//处理文件内容
		if(in.is_open())
		{
			//循环取出每一行数据
			while(getline(in, strRow))
			{
				istringstream stream(strRow);

				//将一行的多个字段取出
				vector<string> RowContext;

				string field("");

				//读取每个字段
				while(getline(stream, field, '='))
				{
					//存入
					RowContext.push_back(field);
				}

				if (RowContext.size() != 2)
				{
					continue;
				}

				items_.insert(make_pair(RowContext[0], RowContext[1]));
			}

			in.close();
		}
	}

	// 保存至文件
	void Save(const char* filename)
	{
		ofstream os(filename, ios::out);

		if (os)
		{
			foreach(items_, it, MAPITEM)
			{
				stringstream ss;

				ss<<it->first<<"="<<it->second;

				os<<ss.str()<<endl;

				ss.clear();
			}

			os.close();
		}
	}

	// 读取一个配置项 
	template<class T>
	T GetItem(const string& key, const T& default_value) const
	{
		T ret = default_value;

		map<string, string>::const_iterator it = items_.find(key);

		if (it != items_.end())
		{
			ret = convert<string, T>(it->second); 
		}

		return ret;
	}

	// 设置一个配置项 
	template<class T>
	void SetItem(const string& key, const T& value)
	{
		map<string, string>::iterator it = items_.find(key);

		string str = convert<T, string>(value);

		if (it != items_.end())
		{
			items_.erase(it);
		}

		items_.insert(make_pair(key, str));
	}

	// 设置一个配置项 
	template<class T>
	void GetArray(const string& key, vector<T>& vtr, char delimiter)
	{
		map<string, string>::iterator it = items_.find(key);

		string str("");

		if (it != items_.end())
		{
			str = it->second;
		}
		else
		{
			return;
		}

		istringstream stream(str);

		string field("");

		//读取每个字段
		while(getline(stream, field, delimiter))
		{
			T value = convert<string, T>(field);

			//存入
			vtr.push_back(value);
		}
	}

	// 设置一个配置项 
	template<class T>
	void SetArray(const string& key, const vector<T>& vtr, char delimiter)
	{
		stringstream ss;

		vector<T>::const_iterator itEnd = vtr.begin();

		foreach(vtr, it, vector<T>)
		{
			itEnd++;

			ss<<*it;

			if (itEnd != vtr.end())
			{
				ss<<delimiter;
			}
		}

		//items_.insert(make_pair(key, ss.str()));
		SetItem<string>(key, ss.str());
	}

	DataHandler& operator=(const DataHandler& value)
	{
		this->items_ = value.items_;

		return *this;
	}

private:
	typedef map<string, string> MAPITEM;
	map<string, string> items_;
};

// 工具类，包含通用函数
class CommonUtil
{
public:
	// 格式化浮点
	static string FormatDouble(double d)
	{
		string str = "";

		char buf[64] = {0};

		sprintf(buf, "%.2f", d);

		str = buf;

		return str;
	}

};

#endif