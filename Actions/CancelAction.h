#pragma once
#include "Actions//Actions.h"

class CancelAction : public Actions
{
private :
	int ID;
public:
	CancelAction(int TC, int ID);
	virtual void ACT();
};

