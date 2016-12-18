#include "StdAfx.h"
#include "FileLogRecorder.h"

CFileRecorder::CFileRecorder(const string &logName) : m_logName(logName)
{
}

CFileRecorder::~CFileRecorder()
{
}

void CFileRecorder::Output(const string &msg)
{
    try
    {
        //设置文件的打开参数
        FILE *outFile = fopen(m_logName.c_str(), "a");
        string msLine = msg.c_str();

        //在文件末尾插入新纪录
        fseek(outFile, 0, SEEK_END);
        fwrite(msg.c_str(), sizeof(char), msg.length(), outFile);
        
        fclose(outFile);
    }
    catch(...)
    {
        
    }
}