
#ifndef _FILE_LOG_RECORDER_H
#define _FILE_LOG_RECORDER_H

#pragma once

#include "FrameworkCls.h"

using namespace fmkcls;

class CFileRecorder : public LogRecorder
{
public:
    CFileRecorder(const string &logName);
    ~CFileRecorder();

    virtual void Output(const string &msg);

private:
    string m_logName;
};

#endif
