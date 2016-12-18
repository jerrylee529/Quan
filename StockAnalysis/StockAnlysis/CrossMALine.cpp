#include "StdAfx.h"
#include "CrossMALine.h"

CrossMALine::CrossMALine(int nCycle)
{
	maline_ = new MALine(nCycle);		
}

CrossMALine::~CrossMALine(void)
{
	delete maline_;
}

void CrossMALine::Calculate(const Quot& quot)
{
	maline_->Calculate(quot);

	int avg = maline_->GetLatestClose();

	if ((quot.Close > avg) && (quot.Open < avg) && (quot.Close > quot.Open))
	{
		date = quot.getDate();

		notifyObservers((void*)date.c_str());			
	}
}
