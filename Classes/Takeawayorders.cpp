#include "Takeawayorders.h"

Takeawayorders::Takeawayorders(int tq, int id, int sz, double pr) : Orders(TYPE_OT ,tq , id , sz , pr)
{
}


int Takeawayorders::getTW() const {
    return getTi() + getTC();
}

//Ti = (Ta - Tq) + (Ts - TR) -> in takeaway TS-TR = 1 (packing timestep)
int Takeawayorders::getTi() const {
    return (TA - TQ) + 1;
}

void Takeawayorders::print(ostream& os) const
{
    if (getAssignedChef() != nullptr)
    {
        os << "[" << getID() << "," << getAssignedChef()->getID() << "]";
    }

    else os << getID();
        
}
