#pragma once
#include "Actions.h"

class CancelAction : public Actions
{
private :
	int ID;
public:
	CancelAction(Restaurant* r ,int TC, int ID);
	virtual void ACT();
};

