/**
** Copyright (C) 2009-2010 ECT. All rights reserved.
**
** This file is part of the Mini-Viewer.
**
** Licensees holding a valid license agreement ECT Trolltech or any of its
** authorized distributors may use this file in accordance with
** the License Agreement provided with the Licensed Software.
**
** See http://www.ectworks.com/pricing.html or email sales@ectworks.com for
**   information about ECT's Commercial License Agreements.
**
** Contact info@ectworks.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _FRAMEWORK_CLS_H
#define _FRAMEWORK_CLS_H

#include <vector>
#include <algorithm>
#include <map>
#include <stack>
#include <string>
#include <list>
#include <queue>
#include "windows.h"
#include "time.h"
#include <assert.h>
#include <fstream>
#include "Utilities.h"

using namespace std;

#define DS_POSCHANGED 0x01000001

namespace fmkcls
{
    // 定义消息ID
    const int DM_ADD    = 0x0001;
    const int DM_DELETE = 0x0002;
    const int DM_CLEAR  = 0x0003;

    // 消息结构
    typedef struct tagDMMsg
    {
        int msgId;
        void *data;
    } DMMsg;

    //  事件监听模板
    template<typename SENDER, typename PARAM = DMMsg>
    class EventListener
    {
    public:
        virtual void Notify(const SENDER &sender, const PARAM &param)
        {

        }
    };


    // 数据访问锁类
    class AccessLock
    {
    public:
        AccessLock()
        {
            ::InitializeCriticalSection(&m_criticalSec);
        }

        ~AccessLock()
        {
            ::DeleteCriticalSection(&m_criticalSec);
        }

        void Lock()
        {
            ::EnterCriticalSection(&m_criticalSec);
        }

        void Unlock()
        {
            ::LeaveCriticalSection(&m_criticalSec);
        }

    private:
        CRITICAL_SECTION m_criticalSec;
    };

	// 
	template<class T>
	class SafeVariant
	{
	public:
		T Get()
		{
			T ret;

			lock_.Lock();

			ret = variant_;

			lock_.Unlock();

			return ret;
		}

		void Set(const T& value)
		{
			lock_.Lock();

			variant_ = value;

			lock_.Unlock();
		}
	private:
		T variant_;
		AccessLock lock_;
	};


    // 事件发布模板
    template<typename SENDER, typename PARAM = DMMsg>
    class Publisher
    {
    public:
        Publisher()
        {

        }

        ~Publisher()
        {
            m_list.clear();
        }

        void AddListener(EventListener<SENDER, PARAM> *pListener)
        {
            vector<EventListener<SENDER, PARAM>*>::iterator it = find(m_list.begin(), m_list.end(), pListener);
            if (it == m_list.end())
            {
                m_list.push_back(pListener);
            }
        }

        void RemoveListener(EventListener<SENDER, PARAM> *pListener)
        {
            vector<EventListener<SENDER, PARAM>*>::iterator it = find(m_list.begin(), m_list.end(), pListener);
            if (it != m_list.end())
            {
                m_list.erase(it);
            }
        }

        void NotifyAll(const SENDER &sender, const PARAM &param)
        {
            for (vector<EventListener<SENDER, PARAM>*>::iterator it = m_list.begin(); 
                it != m_list.end(); it++)
            {
                (*it)->Notify(sender, param);
            }
        }

    protected:
        vector<EventListener<SENDER, PARAM>*>  m_list;
    };


    /*
    * 数据类模板
    */
    template<typename T>
    class DataModel
    {
    public:
        //
        bool AddItem(const T &item)
        {
            bool ret = false;

            m_lock.Lock();

            vector<T>::iterator it = find(m_list.begin(), m_list.end(), item);
            if (it == m_list.end())
            {
                m_list.push_back(item);

                DMMsg msg;
                msg.msgId = DM_ADD;
                msg.data = (void *)&item;

                m_publisher.NotifyAll(*this, msg);

                ret = true;
            }
            else
            {
                ret = false;
            }

            m_lock.Unlock();

            return ret;
        }

        //
        void RemoveItem(const T &item)
        {
            bool b = false;

            m_lock.Lock();

            vector<T>::iterator it = find(m_list.begin(), m_list.end(), item);
            if (it != m_list.end())
            {
                m_list.erase(it);

                DMMsg msg;
                msg.msgId = DM_DELETE;
                msg.data = (void *)&item;

                m_publisher.NotifyAll(*this, msg);
            }

            m_lock.Unlock();
       }

        //
        T &GetItem(int index)
        {

            m_lock.Lock();

            T &ret = m_list[index];

            m_lock.Unlock();

            return ret;
        }

        //
        int GetCount()
        {
            int ret = 0;
            
            m_lock.Lock();

            ret = m_list.size();

            m_lock.Unlock();

            return ret;
        }

        //
        void Clear()
        {
            bool b = false;

            m_lock.Lock();

            if (!m_list.empty())
            {
                m_list.clear();
                b = true;
            }

            m_lock.Unlock();

            if (b)
            {
                DMMsg msg;
                msg.msgId = DM_CLEAR;
                msg.data = NULL;

                m_publisher.NotifyAll(*this, msg);
            }
        }
     
        // 
        Publisher<DataModel<T>> &GetPublisher()
        {
            return m_publisher;
        }

    protected:
        vector<T> m_list;
        Publisher<DataModel<T>> m_publisher;
        AccessLock m_lock;
    };


    // 数据包编解码类模板
    template<typename T>
    class PacketCodec
    {
    public:
        // 编码
        string &Encode(const T &data)
        {
            m_data = data;

            DoEncode();

            return m_packet;
        }

        // 解码
        bool Decode(const string &packet, T *data)
        {
            m_packet = packet;

            if (DoDecode())
            {
                *data = m_data;

                return true;
            }
            else
            {
                return false;
            }
        }

    protected:
        virtual void DoEncode() = 0;

        virtual bool DoDecode() = 0;

        string m_packet;  

        T m_data;
    };


    // 互斥类
    class Mutex
    {
    public:
        Mutex(bool owned)
        {
            m_hMutex = ::CreateMutex(NULL, owned, NULL);  
            if (m_hMutex == NULL) 
            {
                printf("CreateMutex error: %d\n", GetLastError());
            }            
        }

        ~Mutex()
        {
            ::CloseHandle(m_hMutex);
        }

        void Wait(unsigned int uWaitTime = INFINITE)
        {
            DWORD dwWaitResult = ::WaitForSingleObject(m_hMutex, uWaitTime);

            switch(dwWaitResult)
            {
            case WAIT_OBJECT_0:
                break;
            case WAIT_ABANDONED:
                break;
            case WAIT_TIMEOUT:
                break;
            default:
                break;
            }

            ::ReleaseMutex(m_hMutex);
        }

    private:
        HANDLE m_hMutex;
    };

    //
    // 事件类
    class Event
    {
    public:
        Event(bool bManual = false, const char *name = NULL)
        {
            m_hEvent = ::CreateEvent(NULL, bManual, FALSE, name);  
            if (m_hEvent == NULL) 
            {
                printf("CreateEvent error: %d\n", GetLastError());
            }            
        }

        ~Event()
        {
            ::CloseHandle(m_hEvent);
        }

        void Release()
        {
            ::SetEvent(m_hEvent);
        }

        void Capture()
        {
            ::ResetEvent(m_hEvent);
        }

        void Wait(unsigned int uWaitTime = INFINITE)
        {
            DWORD dwWaitResult = ::WaitForSingleObject(m_hEvent, uWaitTime);

            switch(dwWaitResult)
            {
            case WAIT_OBJECT_0:
                break;
            case WAIT_ABANDONED:
                break;
            case WAIT_TIMEOUT:
                break;
            default:
                break;
            }
        }

        HANDLE GetHandle()
        {
            return m_hEvent;
        }

    private:
        HANDLE m_hEvent;
    };
  

	// 进度类
	class IProgress
	{
	public:
		int maximum()
		{
			return maximum_;
		}

		void set_maximum(short maximum) 
		{
			maximum_ = maximum;
		}

		virtual void UpdateProgress(int progress) = 0;

		//virtual void KickOff() = 0;

		virtual void Done() = 0;

		virtual bool IsCanceled() = 0;

	private:
		short maximum_;
	};


    // 命令类
    class Task
    {
    public:
        virtual void Execute() = 0;

		virtual void set_progress(IProgress* progress)  = 0;
    };


    // 对象池
    template<typename T, bool owned=true>
    class ObjPool
    {
    public:
        void AddObject(const string &name, T *obj)
        {
            map<string, T*>::iterator it = mapObj.find(name);

            if (it == mapObj.end())
            {
                mapObj.insert(pair<string, T*>(name, obj));
            }
        }

        void Clear()
        {
            if (owned)
            {
                for (map<string, T*>::iterator it = mapObj.begin(); it != mapObj.end();
                    it++)
                {
                    T *p = (T *)(it->second);
                    if (p != NULL)
                    {
                        delete p;
                    }
                }
            }

            mapObj.clear();
        }

        T *GetObject(const string &name)
        {
            map<string, T*>::iterator it = mapObj.find(name);

            if (it != mapObj.end())
            {
                return it->second;
            }
            else
            {
                return NULL;
            }
        }

    private:
        map<string, T*> mapObj;
    };


    // 构造对象方法
    typedef void* (*CreateFunc)();

    // 对象构造工厂
    class Factory
    {
    public:
        void Register(const string &name, CreateFunc createFunc)
        {
            map<string, CreateFunc>::iterator it = m_mapFunc.find(name);

            if (it == m_mapFunc.end())
            {
                m_mapFunc.insert(make_pair(name, createFunc));
            }
        }

        void *CreateObject(const string &name)
        {
            map<string, CreateFunc>::iterator it = m_mapFunc.find(name);

            if (it != m_mapFunc.end())
            {
                CreateFunc func = it->second;
                return func();
            }
            else
            {
                return NULL;
            }
        }

        void Clear()
        {
            m_mapFunc.clear();
        }
    private:
        map<string, CreateFunc> m_mapFunc;
    };


    /*
    * 共享堆栈类
    */
    template<typename T>
    class ShareStack
    {
    public:
        // 压栈
        void Push(const T &value)
        {
            m_lock.Lock();
            
            m_stack.push(value);

            m_lock.Unlock();
        }

        // 出栈, 返回的数据
        T Popup()
        {
            T ret;

            m_lock.Lock();

            if (!m_stack.empty())
            {
                ret = m_stack.top();

                m_stack.pop();
 
                printf("Stack size: %d\n", m_stack.size());
            }

            m_lock.Unlock();

            return ret;
        }

        bool empty()
        {
            bool bRet;

            m_lock.Lock();

            bRet = m_stack.empty();

            m_lock.Unlock();

            return bRet;
        }

    private:
        stack<T> m_stack;
        AccessLock m_lock;
    };

    /*
    * 共享队列类
    */
    template<typename T>
    class ShareQueue
    {
    public:
        // 压栈
        void Push(const T &value)
        {
            m_lock.Lock();
            
            m_queue.push(value);

            m_lock.Unlock();
        }

        // 出队, 返回的数据
        T Popup()
        {
            T ret;

            m_lock.Lock();

            if (!m_queue.empty())
            {
                ret = m_queue.front();

                m_queue.pop();
            }

            m_lock.Unlock();

            return ret;
        }

        bool empty()
        {
            bool bRet;

            m_lock.Lock();

            bRet = m_queue.empty();

            m_lock.Unlock();

            return bRet;
        }

		void clear()
		{
			m_lock.Lock();

			while(!m_queue.empty())
			{
				m_queue.pop();
			}

			m_lock.Unlock();
		}

    private:
        queue<T> m_queue;
        AccessLock m_lock;
    };

    /*
    * 共享vector类
    */
    template<typename T>
    class ShareVector
    {
    public:
        // 添加item
        void AddItem(const T &value)
        {
            m_lock.Lock();

            m_list.push_back(value);

            m_lock.Unlock();
        }

		void RemoveItem(const T& value)
		{
			m_lock.Lock();

			for(vector<T>::iterator it = m_list.begin(); it != m_list.end(); it++)
			{
				if (*it == value)
				{
					m_list.erase(it);

					break;
				}
			}

			m_lock.Unlock();
		}

        // 根据索引获取item
        T GetItem(size_t index)
        {
            T ret;

            m_lock.Lock();

            ret = m_list.at(index);

            m_lock.Unlock();

            return ret;
        }

        // 获取item数量
        size_t GetCount()
        {
            size_t ret;

            m_lock.Lock();

            ret = m_list.size();
            
            m_lock.Unlock();

            return ret;
        }

		void Clear()
		{
			m_lock.Lock();

			m_list.clear();

			m_lock.Unlock();
		}

    private:
        vector<T> m_list;
        AccessLock m_lock;
    };

    /*
    * 共享数据类
    */
    template<typename T>
    class ShareData
    {
    public:
        // 取值
        T Get()
        {
            T ret;

            m_lock.Lock();

            ret = m_data;

            m_lock.Unlock();

            return ret;
        }

        // 赋值
        void Set(const T &value)
        {
            m_lock.Lock();

            m_data = value;

            m_lock.Unlock();
        }

    private:
        T m_data;
        AccessLock m_lock;
    };


    /*
    * 定义固定长度字符串模板, 长度为_size
    */
    template<unsigned int _size = 256>
    struct FixedSTR
    {
        char value[_size];

        FixedSTR()
        {
            memset(value, 0, sizeof(value));
        }

        FixedSTR(const FixedSTR<_size> &str)
        {
            memcpy(value, str.value, sizeof(value));
        }

        FixedSTR<_size> &operator=(const FixedSTR<_size> &str)
        {
            if (&str != this)
            {
                memcpy(value, str.value, sizeof(value));
            }

            return *this;
        }

        FixedSTR<_size> &operator=(const string &str)
        {
            memcpy(value, str.c_str(), min(sizeof(value), str.length()));

            return *this;
        }

        bool operator==(const FixedSTR<_size> &str) const
        {
            return stricmp(value, str.value) == 0;
        }

        bool operator==(const string &str) const
        {
            return stricmp(value, str.c_str()) == 0;
        }

        bool operator==(const char *str) const
        {
            return stricmp(value, str) == 0;
        }

        unsigned int GetSize()
        {
            return _size;
        }
    };


    /*
    * url解析类
    */
    const int NOPOS = -1;

    class URLParser
    {
        typedef std::string Stringt;
    public:
        URLParser(const Stringt &url)
            : url_(url),
            protpos_(NOPOS),
            dnpos_(NOPOS),
            userpos_(NOPOS),
            pwendpos_(NOPOS),
            portpos_(NOPOS),
            pathpos_(NOPOS),
            filepos_(NOPOS),
            parampos_(NOPOS)
        {
            parseurl();
        }

        URLParser(const char *url)
            : url_(url),
            protpos_(NOPOS),
            dnpos_(NOPOS),
            userpos_(NOPOS),
            pwendpos_(NOPOS),
            portpos_(NOPOS),
            pathpos_(NOPOS),
            filepos_(NOPOS),
            parampos_(NOPOS)
        {
            parseurl();
        }

        Stringt geturl()
        {
            return url_;
        }

        Stringt getprotocol()
        {
            if(protpos_ != NOPOS)
                return url_.substr(0, protpos_);
            else
                return "";
        }

        Stringt getdomain()
        {
            if(dnpos_ != NOPOS && portpos_ != NOPOS)
                return url_.substr(dnpos_, portpos_ - 1 - dnpos_);
            else if(dnpos_ != NOPOS && pathpos_ != NOPOS)
                return url_.substr(dnpos_, pathpos_ - dnpos_);
            else
                return "";
        }

        Stringt getport()
        {
            if(portpos_ != NOPOS && pathpos_ != NOPOS)
                return url_.substr(portpos_, pathpos_ - portpos_);
            else
                return "";
        }

        Stringt getpath()
        {
            if(pathpos_ != NOPOS && filepos_ != NOPOS)
                return url_.substr(pathpos_, filepos_ - pathpos_);
            else if(pathpos_ != NOPOS && parampos_ != NOPOS)
                return url_.substr(pathpos_, parampos_ - 1 - pathpos_);
            else if(pathpos_ != NOPOS)
                return url_.substr(pathpos_, url_.length() - 1);
            else
                return "";
        }

        Stringt getfile()
        {
            if(filepos_ != NOPOS && parampos_ != NOPOS)
                return url_.substr(filepos_, parampos_ - 1 - filepos_);
            else if(filepos_ != NOPOS)
                return url_.substr(filepos_, url_.length() - 1);
            else
                return "";
        }

        Stringt getparameter()
        {
            if(parampos_ != NOPOS && anchorpos_ != NOPOS)
                return url_.substr(parampos_, anchorpos_ - 1 - parampos_);
            else if(parampos_ != NOPOS)
                return url_.substr(parampos_, url_.length() - 1);
            else
                return "";
        }

        Stringt getanchor()
        {
            if(anchorpos_ != NOPOS)
                return url_.substr(anchorpos_, url_.length() - 1);
            else
                return "";
        }

        Stringt gethost()
        {
            return url_.substr(dnpos_, pathpos_ - dnpos_);
        }

        Stringt getreferer()
        {
            return url_.substr(0, pathpos_);
        }

    private:
        void parseurl()
        {
            Stringt::size_type beginpos = 0, endpos = 0;

            beginpos = url_.find(':', 0);
            endpos = url_.find('/', 0);
            if(beginpos != Stringt::npos && beginpos < endpos && beginpos < url_.find('.', 0))
            {
                protpos_ = beginpos;
                beginpos = url_.find('/', endpos + 1) + 1;
                dnpos_ = beginpos;
            }
            else
            {
                dnpos_ = 0;
                beginpos = dnpos_;
            }

            endpos = url_.find(':', beginpos);
            if(endpos != Stringt::npos)
            {
                portpos_ = endpos + 1;
                beginpos = endpos;
            }

            endpos = url_.find('/', beginpos);
            if(endpos != Stringt::npos)
            {
                pathpos_ = endpos;
            }
            else
            {
                url_ = url_ + '/';
                endpos = url_.find('/', beginpos);
                pathpos_ = endpos;
            }

            beginpos = endpos;
            endpos = url_.find('.', beginpos);
            if(endpos != Stringt::npos)
            {
                beginpos = url_.rfind('/', endpos) + 1;
                filepos_ = beginpos;
            }

            beginpos = url_.find('?', beginpos);
            if(beginpos != Stringt::npos)
            {
                parampos_ = beginpos + 1;
            }

            beginpos = url_.rfind('#', url_.length() - 1);
            if(beginpos != Stringt::npos)
            {
                anchorpos_ = beginpos + 1;
            }
        }

    private:
        Stringt url_;
        size_t protpos_;
        size_t dnpos_;
        size_t userpos_;
        size_t pwendpos_;
        size_t portpos_;
        size_t pathpos_;
        size_t filepos_;
        size_t parampos_;
        size_t anchorpos_;
    };


    /*
    * 进程处理类
    */
    class Processer : public Task
    {
    public:
        void SetProcessName(const string &name)
        {
            m_name = name;
        }

        void SetParameters(const char **params, int count)
        {
            m_params.clear();

            for (int i = 0; i < count; i++)
            {
                m_params.push_back(params[i]);
            }
                                
        }

        virtual void Execute()
        {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            memset(&si, 0, sizeof(si));
            si.cb = sizeof(si);
            memset(&pi, 0, sizeof(pi));

            string str = GetProcessName();

            // Start the child process. 
            if( !CreateProcess( NULL,   // No module name (use command line). 
                (LPSTR)str.c_str(), // Command line. 
                NULL,             // Process handle not inheritable. 
                NULL,             // Thread handle not inheritable. 
                FALSE,            // Set handle inheritance to FALSE. 
                0,                // No creation flags. 
                NULL,             // Use parent's environment block. 
                NULL,             // Use parent's starting directory. 
                &si,              // Pointer to STARTUPINFO structure.
                &pi )             // Pointer to PROCESS_INFORMATION structure.
                ) 
            {
                printf( "CreateProcess failed (%d).\n", GetLastError() );
                return;
            }

            m_hProcess = pi.hProcess;
            m_hThread = pi.hThread;

            // Wait until child process exits.
            //WaitForSingleObject(pi.hProcess, INFINITE);

            // Close process and thread handles. 
            //CloseHandle(pi.hProcess);
            //CloseHandle(pi.hThread);
        }

        void Terminate()
        {
            TerminateProcess(m_hProcess, 0);

            CloseHandle(m_hProcess);
            CloseHandle(m_hThread);
        }
    private:
        string GetProcessName()
        {
            string ret;
            ret = m_name;
            for (vector<string>::iterator it = m_params.begin(); 
                it != m_params.end(); it++)
            {
                ret += " " + *it;
            }

            return ret;
        }

        string m_name;
        vector<string> m_params;
        HANDLE m_hProcess, m_hThread;
    };


    // 线程类
    class CTaskThread
    {
    public:
        CTaskThread::CTaskThread()
            :m_hEvent(NULL), m_hThread(NULL)
        {

        }

        virtual CTaskThread::~CTaskThread()
        {

        }

        void CTaskThread::Start(DWORD dwCreateFlag = 0, int stacksize = 4096)
        {
            if (IsRunning())
            {
                return;
            }

            m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

            //assert(m_hEvent != NULL);

            if (m_hEvent == NULL)
            {
                // log error
                return;
            }

            DWORD dwID;

            m_hThread = CreateThread(NULL, stacksize, ThreadProc, this, 
                (stacksize==0)?dwCreateFlag:STACK_SIZE_PARAM_IS_A_RESERVATION, &dwID);

            //assert(m_hThread != NULL);

            if (m_hThread == NULL)
            {
                // log error
            }
        }

        // bAutoRelease: true代表自动此释放线程对象, 这样将不会使调用者等待线程结束，否则相反
        void CTaskThread::Stop(int nWait = INFINITE)
        {
            if (!IsRunning())
            {
                return;
            }

            SetEvent(m_hEvent);  

            WaitForSingleObject(m_hThread, nWait);

            CloseHandle(m_hEvent);
            m_hEvent = NULL;

            CloseHandle(m_hThread);
            m_hThread = NULL;
        }

        bool CTaskThread::IsRunning()
        {
            return (m_hThread != NULL)?true:false;
        }

        void CTaskThread::SetPriority(int nPriority)
        {
            SetThreadPriority(m_hThread, nPriority);
        }

        void CTaskThread::Suspend()
        {
            SuspendThread(m_hThread);
        }

        void CTaskThread::Resume()
        {
            ResumeThread(m_hThread);
        }

    protected:
        virtual int Run() = 0;

    private:
        static DWORD WINAPI CTaskThread::ThreadProc(LPVOID lpParam)
        {
            CTaskThread* pThread = (CTaskThread*)lpParam;
            while (1)
            {
                // 收到事件通知则退出循环
                if (WaitForSingleObject(pThread->m_hEvent, 50) != WAIT_TIMEOUT)
                {
                    break;
                }

                if (pThread->Run())
                {
                    break;
                }
            }

            return 0;
        }

    protected:
        HANDLE  m_hEvent;
        HANDLE  m_hThread;
    };


    // 所有任务类的基类，本类为纯虚类，子类需集成后实现execute虚方法
    class CTask
    {
    public:
        CTask::CTask(void)
            : m_nTag(0)
        {
        }

        virtual CTask::~CTask(void)
        {
        }


    public:
        void SetTag(int tag)
        {
            m_nTag = tag;
        }

        int GetTag()
        {
            return m_nTag;
        }

        // 任务执行函数
        virtual void Execute() = 0;

        bool operator==(const CTask& task)
        {
            return (task.m_nTag == m_nTag)?true:false;
        }
    private:
        int m_nTag;
    };

    // 任务处理器，在线程中按顺序处理任务
    class CTaskProcesser : public CTaskThread
    {
	public:
        CTaskProcesser::CTaskProcesser()
        {
        }

        CTaskProcesser::~CTaskProcesser()
        {
        }

        void CTaskProcesser::AddTask(CTask* pTask)
        {
            m_tasks.Push(pTask);
        }

        bool CTaskProcesser::Empty()
        {
            return m_tasks.empty();
        }

        int CTaskProcesser::Run()
        {
            while (!Empty())
            {
                CTask* pTask = m_tasks.Popup();

                if (pTask)
                {
                    pTask->Execute();
                }

                if (WaitForSingleObject(m_hEvent, 50) == WAIT_OBJECT_0)
                {
                    break;
                }
            }

            return 0;
        }

    private:
        ShareQueue<CTask*> m_tasks; 
        AccessLock m_lock;
    };

    /*
    * 线程类
    * 以下包括线程池
    */

    /*
    * CBaseThread                           
    * This class is base class of all the thread class                                     
    */
    class BaseThread : public CTaskThread
    {
    public:
		BaseThread() : m_pTask(0)
		{
		}

        // 设置线程中需要执行的命令
        void SetTask(Task *pTask)
        {
            m_lock.Lock();

            m_pTask = pTask;

            m_lock.Unlock();
        }

        bool IsFree()
        {
            bool bRet;

            //m_lock.Lock();

            bRet = (m_pTask == 0)?true:false;

            //m_lock.Unlock();

            return bRet;
        }

    protected:
        virtual int Run()
        {
            m_lock.Lock();

            if (m_pTask != NULL)
            {
                m_pTask->Execute();
            }

            m_pTask = NULL;

            m_lock.Unlock();

            return 0;
        }

        Task *m_pTask;
        AccessLock m_lock;
    };


    /*
    * 线程池类
    */ 

    class ThreadPool : public Singleton<ThreadPool>, public CTaskThread
    {
        friend class Singleton<ThreadPool>;
    public:
        // 增加一个任务
        void AddTask(Task *pTask)
        {
            m_tasks.Push(pTask);
        }

        // 获取线程数
        int GetCount()
        {
            return (int)m_threads.size();
        }

    protected:
        ThreadPool()
        {
			SYSTEM_INFO si;
			GetSystemInfo(&si);
            for (DWORD i=0; i<si.dwNumberOfProcessors; i++)
            {
                m_threads.push_back(BaseThread());    
            }
        }

        virtual int Run()
        {
            while (!m_tasks.empty())
            {
                for (MTVECTOR::iterator it = m_threads.begin(); it != m_threads.end(); it++)
                {
                    if (it->IsFree())
                    {
                        it->SetTask(m_tasks.Popup());
                        break;
                    }
                }
            }

            return 0;
        }
    
    private:
        typedef vector<BaseThread> MTVECTOR;
        MTVECTOR m_threads;
        ShareQueue<Task*> m_tasks;
    };

    /****************************************************************************************/
    /* ends the thread pool                                                                 */
    /****************************************************************************************/


    /*
    class Socket 
    {
    public:
        Socket() : _socket(INVALID_SOCKET), _port()
        {}

        Socket(Socket& sock) : _socket(INVALID_SOCKET), _port()
        {
            create(sock._socket, (SOCKADDR &)sock._addrIn);	
        }

        virtual ~Socket()
        {
            close();
        }

        // create/release
        bool create(SOCKET & sock, SOCKADDR& addr)
        {
            if (sock == INVALID_SOCKET)
                return false;

            // get new socket
            memcpy(&_addrIn, (SOCKADDR *)&addr, sizeof(addr));
            memcpy(&_socket, &sock, sizeof(_socket));

            return true;
        }

        void release()
        {
            // close socket if open
            if (_socket != INVALID_SOCKET)
            {
                closesocket(_socket);
                _socket = INVALID_SOCKET;
            }
        }

        bool isValid()
        {
            if (_socket != INVALID_SOCKET)
                return true;
            else
                return false;
        }

        long getPort()
        {
            return _port;
        }

        // open methods
        static bool initSockAddrIn(string & host, long port, SOCKADDR_IN& addrIn)
        {
            // init socket in struct
            memset(&addrIn,0,sizeof(addrIn));

            // set family
            addrIn.sin_family = AF_INET;


            if (!host.empty())
            {
                // retrieve the host data corresponding to the host name
                LPHOSTENT phost;
                if ((phost = gethostbyname(host.c_str())) != NULL) 
                {
                    // assign the socket IP address. 
                    memcpy ( (char FAR *) &(addrIn.sin_addr), 
                        phost->h_addr, 
                        phost->h_length );
                }
                else
                {
                    // get inet address
                    addrIn.sin_addr.s_addr = inet_addr(host.c_str());
                }
            }
            else
            {
                // set inet address
                addrIn.sin_addr.s_addr = htonl(INADDR_ANY);
            }

            // set port
            addrIn.sin_port = htons(port);

            return true;
        }

        bool open(string host, long port)
        {
            // get socket
            _socket = socket(PF_INET,SOCK_STREAM,0);

            if (_socket == INVALID_SOCKET)
                return false;

            // init address in
            _port = port;
            initSockAddrIn(host, port, _addrIn);

            // connect to socket
            if (connect(_socket, (const SOCKADDR *) &_addrIn, sizeof(_addrIn)) == SOCKET_ERROR)
            {
                int error = getLastError();
                return false;
            }


            return true;
        }

        // close
        void close()
        {
            // close socket if open
            if (_socket != INVALID_SOCKET)
            {
                closesocket(_socket);
                _socket = INVALID_SOCKET;
            }
        }

        static int getLastError()
        {
            return WSAGetLastError();
        }

        static bool init()
        {
#if defined( _WIN32 )

            // init startup
            WSADATA WSAData;
            WSAStartup(MAKEWORD(2,0), &WSAData);
#endif
            return true;
        }

        long getRecieveSize()
        {
            // return not of bytes waiting
            // if fail show none
            long noBytes;

            if (ioControl(FIONREAD, (unsigned long *)&noBytes))
                return noBytes;
            else
                return 0;
        }

        bool setSocketNonBlocking()
        {
            // set socket to non-blocking mode
            // probably need to add more ioctl later
            unsigned long ioctl_opt = -1;

            return ioControl( FIONBIO, &ioctl_opt);
        }

        // recieve/send methods
        long send(string& buffer)
        {
            return send((LPTSTR)buffer.c_str(), buffer.size());
        }

        long send(LPCTSTR buffer, DWORD noToWrite)
        {
            return send((LPTSTR)buffer, noToWrite);
        }

        long send(LPTSTR pBuffer, DWORD noToWrite)
        {
            // send to socket
            int noWritten = send( _socket, pBuffer, noToWrite, 0);

            // if error show none sent
            if (noWritten == SOCKET_ERROR)
                return 0;
            else
                return noWritten;
        }

        long recieve(LPTSTR pBuffer, DWORD noToRead)
        {
            // recieve from socket
            int noRead = recv(_socket, pBuffer, noToRead, 0);

            // if no recieve error
            if (noRead == SOCKET_ERROR)
            {
                noRead = -1;
            }
            else
            {
                // else terminate buffer
                pBuffer[noRead] = '\0';
            }

            return noRead;
        }

        bool ioControl(long cmd, u_long * argp)
        {
            // perfom action
            long ret = ioctlsocket(_socket, cmd, argp);

            // if success show it
            // else get error show failed
            if (ret == 0)
                return true;
            else
            {
                // show error
                fprintf(stderr,"ioctlsocket failed %d\n",getLastError());

                return false;
            }
        }


        // operators
        operator SOCKET()
        {
            return _socket;
        }

        Socket& operator<<(string& buffer)
        {
            send( (LPTSTR) buffer.c_str(), buffer.size());
            return *this;
        }

        Socket& operator>>(stringstream& strm)
        {
            send( (LPTSTR) strm.str().c_str(), strm.str().size());
            return *this;
        }

        Socket& operator<<(LPCTSTR buffer)
        {
            send((LPTSTR) buffer, lstrlen(buffer));
            return *this;
        }

        Socket& operator<<(LPTSTR buffer)
        {
            send(buffer, lstrlen(buffer));
            return *this;
        }

        // test if file open
        bool isOpen()
        {
            if (_socket == INVALID_SOCKET)
                return false;
            else
                return true;
        }

    protected:
        SOCKET			_socket;
        SOCKADDR_IN		_addrIn;
        long			_port;
    };
    */

	// 多线程计算模板
	template<class T>
	class MultiThreadCalculator
	{
	public:
		// 运行
		void Execute()
		{
			SYSTEM_INFO si;
			GetSystemInfo(&si);

			for (int i=1; i<si.dwNumberOfProcessors; i++)
			{
				HANDLE hThread = CreateThread(NULL, 0, CalculateProc, this, 0, 0);

				if (hThread != INVALID_HANDLE_VALUE)
				{
					CloseHandle(hThread);
				}
			}
		}

		// 终止
		static void Terminate()
		{
			s_terminated_ = true;

			while(s_threads_.GetCount() > 0)
			{
				Sleep(50);
			}

			s_terminated_ = false;
		}

	protected:
		virtual void Run() = 0;

		virtual void Done() = 0;

		static DWORD WINAPI CalculateProc(void* lpParam)
		{
			T* mtc = (T*)lpParam;

			// 线程队列增加一个线程
			s_threads_.AddItem(mtc);

			while(!s_terminated_)
			{
				// 执行计算
				try
				{
					mtc->Run();
				}
				catch (...)
				{
					continue;
				}

				Sleep(50);
			}

			// 全部完成则发送信息
			if (s_threads_.GetCount() == 1)
			{
				mtc->Done();
			}

			// 释放对象
			delete mtc;

			// 线程队列删除一个线程
			s_threads_.RemoveItem(mtc);

			return 0;
		}

		static bool s_terminated_;

		static ShareVector<T*> s_threads_;  
	};


	template<class T>
	ShareVector<T*> MultiThreadCalculator<T>::s_threads_;

	template<class T>
	bool MultiThreadCalculator<T>::s_terminated_ = false;

}





#endif //_FRAMEWORK_CLS_H



