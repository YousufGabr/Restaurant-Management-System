#include "CancelAction.h"

CancelAction::CancelAction(Restaurant* r ,int TC, int ID)
{
	this->RestPtr = r;
	this->currentTimestep = TC;
	this->ID = ID;
}

void CancelAction::ACT()
{
	//RestPtr->CancelOVC(ID);
}
