#pragma once
#include "Classes/orders.h"
#include "Classes/Resturant.h"

enum ACT_TYPE {
    Q,  //request
	X, //cancel

};

class Actions
{
protected:
	int currentTimestep;
	Resturant* RestPtr;
public :
	Actions() : RestPtr(nullptr) , currentTimestep(0) {}
	virtual void  ACT() = 0;
};

