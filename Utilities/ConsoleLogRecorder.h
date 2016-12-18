
#ifndef _CONSOLE_LOG_RECORDER_H
#define _CONSOLE_LOG_RECORDER_H

#pragma once

#include "FrameworkCls.h"

using namespace fmkcls;

class CConsoleRecorder : public LogRecorder
{
public:
    CConsoleRecorder(void);
public:
    ~CConsoleRecorder(void);

    virtual void Output(const string &msg);
};


#endif
