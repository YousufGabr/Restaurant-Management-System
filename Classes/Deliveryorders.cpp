#include "Deliveryorders.h"

Deliveryorders::Deliveryorders(ORD_TYPE type,int tq, int id, int sz, double pr, int distance , int TH)
    : Orders(type, tq, id, sz, pr), distance (distance), AssignedScooter(nullptr), TS(0), TH(TH)
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
    return (price * 0.5) / (size * 0.3 + distance * 0.2 + 1);
}

//To be revised
int Deliveryorders::getServicePriority() const
{
    return -1 * (distance / (AssignedScooter->getSpeed()));
}

int Deliveryorders::getDeliveryDuration() const
{
    return (distance / (AssignedScooter->getSpeed()));
}

void Deliveryorders::print(ostream& os) const
{
    if (getAssignedChef() != nullptr)
    {
        os << "[" << getID() << "," << getAssignedChef()->getID() << "]";
    }
    else if (getAssignedScooter() != nullptr)
    {
        os << "[" << getID() << ",S" << getAssignedScooter()->getID() << "]";
    }
    else os << getID();
}
