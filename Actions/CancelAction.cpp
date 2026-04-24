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

std::ostream& operator<<(std::ostream& os, const CancelAction* X)
{
	os << "( X, " << X->currentTimestep << ", " << X->ID << " )" << endl;
	return os;
}
