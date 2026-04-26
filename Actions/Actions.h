#pragma once
class Restaurant;
#include <iostream>
using namespace std;

enum ACT_TYPE {
    Q,  //request
	X, //cancel
};

class Actions
{
protected:
	int currentTimestep;
	Restaurant* RestPtr;
	int ID;
	virtual void print(ostream& os) const = 0;
public :
	Actions() : RestPtr(nullptr) , currentTimestep(0), ID(0) {}
	virtual void  ACT() = 0;
	int getTimestep() const { return currentTimestep;}
	friend ostream& operator<<(ostream& os, const Actions* act) {
		act->print(os);
		return os;
	}
};

