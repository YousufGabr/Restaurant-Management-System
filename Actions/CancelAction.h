#pragma once
#include "Actions.h"
#include<ostream>

class CancelAction : public Actions
{
private :
	int ID;
public:
	CancelAction(Restaurant* r ,int TC, int ID);
	virtual void ACT();
	friend std::ostream& operator<<(std::ostream& os, const CancelAction* X);
};


