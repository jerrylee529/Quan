#pragma once

#include <map>
#include <deque>
#include <vector>
#include "StockDataStructures.h"
#include "Utilities.h"

using namespace std;

class Algorithm : public Observer
{
public:
	Algorithm(void);
	virtual ~Algorithm(void);

public:
	virtual void Calculate(const Quot& quot) {}

	virtual void done() {}

	void update(Observable* observable, void* arg)
	{
		Quot* pQuot = (Quot*)arg;

		Calculate(*pQuot);
	}
};

//
class Trigger : public Observable, public Algorithm
{
public:
	void TriggerSomething(){}
};