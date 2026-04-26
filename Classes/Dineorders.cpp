#include "Dineorders.h"


Dineorders::Dineorders(ORD_TYPE type ,int tq, int id, int sz, double pr, int seats, int duration, bool sharable)
    : Orders(type ,tq, id, sz, pr), noOfSeats(seats), orderDuration(duration), canShare(sharable),
    AssignedTable(nullptr), TS(0) 
{
}

int Dineorders::getorderDuration() const
{
    return orderDuration;
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
    return getTi() + getTC();
}

//priority , To be revised
int Dineorders::getServicePriority()
{
    return -1 * orderDuration;
}


void Dineorders::print(ostream& os) const
{
    if (getAssignedChef() != nullptr)
    {
        os << "[" << getID() << "," << getAssignedChef()->getID() << "]";
    }

    else if (getAssignedTable() != nullptr)
    {
        os << "[" << getID() << ",T" << getAssignedTable()->getID() << "]";
    }
    else os << getID();
}
