#include "Deliveryorders.h"

Deliveryorders::Deliveryorders(ORD_TYPE type,int tq, int id, int sz, double pr, int distance)
    : Orders(type, tq, id, sz, pr), distance (distance), AssignedScooter(nullptr), TS(0)
{
}


void Deliveryorders::setAssignedScooter(Scooters* sct) { AssignedScooter = sct; }
void Deliveryorders::setDeliveryDistance(int dist)
{
    this->distance = dist;
}

Scooters* Deliveryorders::getAssignedScooter() const { return AssignedScooter; }
int Deliveryorders::getDistance() const { return distance; }

int Deliveryorders::getTserv() const { return TF - TS; }
int Deliveryorders::getTW() const {
    return getTi() + getCookPeriod();
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


std::ostream& operator<<(std::ostream& os, const Deliveryorders* OV)
{
    if (OV->getAssignedChef() != nullptr)
    {
        os << "[" << OV->getID() << "," << OV->getAssignedChef()->getID() << "]";
    }
    else if (OV->getAssignedScooter() != nullptr)
    {
        os << "[" << OV->getID() << ",S" << OV->getAssignedScooter()->getID() << "]";
    }
    return os;
}
