#include "StdAfx.h"
#include "CrossWeekLineStrategy.h"

CrossWeekLineStrategy::CrossWeekLineStrategy(const DataHandler& datahandler)
: Strategy(datahandler)
{
}

CrossWeekLineStrategy::~CrossWeekLineStrategy(void)
{
}

void CrossWeekLineStrategy::Initialize()
{

}

Strategy::ACTION CrossWeekLineStrategy::Trigger(const Quot& quot)
{
	// ¼ÆËãÖÜÏß
	weekline_->Calculate(quot);

	//

	return SELL;
}