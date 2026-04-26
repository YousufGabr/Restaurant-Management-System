#include "CancelAction.h"
#include "../Restaurant/Restaurant.h"

CancelAction::CancelAction(Restaurant* r ,int TC, int ID)
{
	this->RestPtr = r;
	this->currentTimestep = TC;
	this->ID = ID;
}

void CancelAction::ACT()
{
	RestPtr->CancelOVC(ID);
}

void CancelAction::print(ostream& os) const
{
	os << "[X," << currentTimestep << "," << ID << "] ";
}
