#pragma once
class Restaurant;

enum ACT_TYPE {
    Q,  //request
	X, //cancel
};

class Actions
{
protected:
	int currentTimestep;
	Restaurant* RestPtr;
public :
	Actions() : RestPtr(nullptr) , currentTimestep(0) {}
	virtual void  ACT() = 0;
	int getTimestep() const { return currentTimestep;}
};

