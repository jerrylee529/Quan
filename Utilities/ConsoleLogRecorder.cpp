#include "StdAfx.h"
#include "ConsoleLogRecorder.h"

CConsoleRecorder::CConsoleRecorder(void)
{
}

CConsoleRecorder::~CConsoleRecorder(void)
{
}

void CConsoleRecorder::Output(const string &msg)
{
    printf("%s", msg.c_str());
}