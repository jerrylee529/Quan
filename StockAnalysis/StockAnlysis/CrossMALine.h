/*
*
* 
*/
#pragma once

#include "algorithm.h"
#include "MALine.h"

class CrossMALine :
	public Trigger
{
public:
	CrossMALine(int nCycle);
public:
	~CrossMALine(void);

	virtual void Calculate(const Quot& quot);

	string date;
private:
	MALine* maline_;

};
