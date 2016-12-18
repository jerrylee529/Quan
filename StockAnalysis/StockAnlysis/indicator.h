#pragma once

#include "algorithm.h"


//
class Indicator :
	public Algorithm
{
public:
	Indicator(void);
	virtual ~Indicator(void);
public:
	virtual string name();

	virtual void Calculate(const Quot& quot) = 0;


protected:
	string name_;

};
