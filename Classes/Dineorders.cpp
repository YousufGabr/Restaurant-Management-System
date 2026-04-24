#include "Dineorders.h"


Dineorders::Dineorders(ORD_TYPE type ,int tq, int id, int sz, double pr, int seats, int duration, bool sharable)
    : Orders(type ,tq, id, sz, pr), noOfSeats(seats), orderDuration(duration), canShare(sharable),
    AssignedTable(nullptr), TS(0) 
{
}

void Dineorders::setNoOfSeats(int seats)
{
    this->noOfSeats = seats;
}

void Dineorders::setorderDuration(int t)
{
    this->orderDuration = t;
}

void Dineorders::setTS(int t)
{
    this->TS = t;
}

bool Dineorders::isSharable() const { return canShare; }

int Dineorders::getNoOfSeats() const { return noOfSeats; }


void Dineorders::setAssignedTable(Tables* tbl) { AssignedTable = tbl; }

Tables* Dineorders::getAssignedTable() const { return AssignedTable; }


// time periods
int Dineorders::getTserv() const { return TF - TS;}

int Dineorders::getTi() const
{
    return  (TA - TQ) + (TS - TR);
}

int Dineorders::getTW() const {
    return getTi() + getCookPeriod();
}

//priority , To be revised
int Dineorders::getServicePriority()
{
    return -1 * orderDuration;
}


std::ostream& operator<<(std::ostream& os, const Dineorders* OD)
{
    if (OD)
    {
        if (OD->getAssignedChef() != nullptr)
        {
            os << "[" << OD->getID() << "," << OD->getAssignedChef()->getID() << "]";
        }
       
        else if (OD->getAssignedTable() != nullptr)
        {
            os << "[" << OD->getID() << ",T" << OD->getAssignedTable()->getID() << "]";
        }
        else os << OD->getID();
    }
    return os;
}
