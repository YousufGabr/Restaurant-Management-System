#include "Deliveryorders.h"


Deliveryorders::Deliveryorders(ORD_TYPE type,int tq, int id, int sz, double pr, int distance , int TH)
    : Orders(type, tq, id, sz, pr), distance (distance), AssignedScooter(nullptr), TS(0), TH(TH), isow(false)
{
}


void Deliveryorders::setAssignedScooter(Scooters* sct) { AssignedScooter = sct; }
void Deliveryorders::setDeliveryDistance(int dist)
{
    this->distance = dist;
}
void Deliveryorders::setTS(int t)
{
    this->TS = t;
}

Scooters* Deliveryorders::getAssignedScooter() const { return AssignedScooter; }
int Deliveryorders::getDistance() const { return distance; }

int Deliveryorders::getTserv() const { return TF - TS; }
int Deliveryorders::getTW() const {
    return getTi() + getTC();
}
int Deliveryorders::getTi() const
{
    return  (TA - TQ) + (TS - TR);
}

int Deliveryorders::getOVGpriority() const
{
    return int((price * 2.0) / (size + distance / 50.0 + 1) * 100);
}


int Deliveryorders::getServicePriority() const
{
    return -1 * ceil((distance * 1.0 / (AssignedScooter->get_Speed())));
}


int Deliveryorders::getDeliveryDuration() const
{
    return ceil((distance * 1.0/ (AssignedScooter->get_Speed())));
}


void Deliveryorders::print(ostream& os) const
{
    if (getAssignedChef() != nullptr)
    {
        os << "[" << getID() << "," << getAssignedChef()->getID() << "]";
    }
    else if (getAssignedScooter() != nullptr)
    {
        if(getAssignedScooter()->getType() == SC_TYPE::TYPE_NORMAL) os << "[" << getID() << ",S" << getAssignedScooter()->getID() << "]";
		else os << "[" << getID() << ",SR" << getAssignedScooter()->getID() << "]";
        
    }
    else os << getID();
}
