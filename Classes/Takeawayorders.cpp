#include "Takeawayorders.h"

Takeawayorders::Takeawayorders(int tq, int id, int sz, double pr) : Orders(TYPE_OT ,tq , id , sz , pr)
{
}

// there is a timestep for packing so we add 1 instead of TS
int Takeawayorders::getTW() const {
    return getTi() + getCookPeriod();
}

int Takeawayorders::getTi() const {
    return (TA - TQ) - TR + 1;
}

std::ostream& operator<<(std::ostream& os, const Takeawayorders* OT)
{
    if (OT)
    {
        if (OT->getAssignedChef() != nullptr)
        {
            os << "[" << OT->getID() << "," << OT->getAssignedChef()->getID() << "]";
        }
       
        else os << OT->getID();
    }
    return os;
}
