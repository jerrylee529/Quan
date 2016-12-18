#include "StdAfx.h"
#include "TheLargestGain.h"
#include "Utilities.h"

const int INVALID_PRICE = 1000000;

TheLargestGain::TheLargestGain(const char* szBeginDate, const char* szEndDate)
:largestGain(0.0),beginDate(szBeginDate),endDate(szEndDate), open(INVALID_PRICE), close(0)
{
}

TheLargestGain::~TheLargestGain(void)
{
}

void TheLargestGain::Calculate(const Quot& quot)
{
	if (open == INVALID_PRICE)
	{
		open = quot.Open;
	}

	close = quot.Close;

	largestGain = (double)(close - open)*100/open;
}

double TheLargestGain::GetValue()
{
	return largestGain;
}