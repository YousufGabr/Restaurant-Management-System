#include "CancelAction.h"

CancelAction::CancelAction(int TC, int ID)
{
	this->currentTimestep = TC;
	this->ID = ID;
}

void CancelAction::ACT()
{
	//RestPtr->CancelOVC(ID);
}
