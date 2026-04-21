#include "Orders.h"
#include "Chefs.h"
#include "Scooters.h"
#include "Tables.h"

Orders::Orders(ORD_TYPE t, int tq, int id, int sz, double pr)
    : ID(id), type(t), TQ(tq), size(sz), price(pr),
    TA(-1), TR(-1), TS(-1), TF(-1), distance(0), noOfSeats(0),
	canShare(false), orderDuration(0), assignedChef(nullptr), AssignedScooter(nullptr), AssignedTable(nullptr)
{
}

void Orders::setDineInInfo(int seats, int duration, bool share) {
    noOfSeats = seats;
    orderDuration = duration;
    canShare = share;
}

void Orders::setDeliveryDistance(int dist) {
    distance = dist;
}

void Orders::setTA(int t) { TA = t; }
void Orders::setTR(int t) { TR = t; }
void Orders::setTS(int t) { TS = t; }
void Orders::setTF(int t) { TF = t; }

// --- Link the chef to the order ---
void Orders::setAssignedChef(Chefs* chf) { assignedChef = chf; }
Chefs* Orders::getAssignedChef() const { return assignedChef; }

void Orders::setAssignedScooter(Scooters* sct) { AssignedScooter = sct; }
Scooters* Orders::getAssignedScooter() const { return AssignedScooter; }

void Orders::setAssignedTable(Tables* tbl) { AssignedTable = tbl; }
Tables* Orders::getAssignedTable() const { return AssignedTable; }

int Orders::getCookPeriod() const { return TR - TA; }
int Orders::getServiceDuration() const { return TF - TS; }
int Orders::getWaitTime() const {
    return (TA - TQ) + (TS - TR);
}

int Orders::getPriority() const {
    if (type != TYPE_OVG) return 0;
    // Weighted priority equation for OVG
    return (price * 0.5) / (size * 0.3 + distance * 0.2 + 1);
    //======== NEED TO IMPROVE DEPENDING ON THE TYPE. ===============
}

int Orders::getID() const { return ID; }
ORD_TYPE Orders::getType() const { return type; }
int Orders::getTQ() const { return TQ; }
int Orders::getSize() const { return size; }
double Orders::getPrice() const { return price; }
bool Orders::isSharable() const { return canShare; }
int Orders::getDistance() const { return distance; }
int Orders::getNoOfSeats() const { return noOfSeats; }
int Orders::getOrderDuration() const { return orderDuration; }

std::ostream& operator<<(std::ostream& os, const Orders* ord) {
    if (ord)
    {
        if (ord->getAssignedChef() != nullptr)
        {
            os << "[" << ord->getID() << "," << ord->getAssignedChef()->getID() << "]";
        }
        else if (ord->getAssignedScooter()!= nullptr)
        {
            os << "[" << ord->getID() << ",S" << ord->getAssignedScooter()->getID() << "]";
        }
        else if (ord->getAssignedTable()!= nullptr)
        {
            os << "[" << ord->getID() << ",T" << ord->getAssignedTable()->getID() << "]";
        }
        else os << ord->getID();
    }
    return os;
}
