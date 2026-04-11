#include "orders.h"

orders::orders(int id, ORD_TYPE t, int tq, int sz, double pr)
    : ID(id), type(t), TQ(tq), size(sz), price(pr),
    TA(-1), TR(-1), TS(-1), TF(-1), distance(0), noOfSeats(0), canShare(false), orderDuration(0) {
}

void orders::setDineInInfo(int seats, int duration, bool share) {
    noOfSeats = seats;
    orderDuration = duration;
    canShare = share;
}

void orders::setDeliveryDistance(int dist) {
    distance = dist;
}

void orders::setTA(int t) { TA = t; }
void orders::setTR(int t) { TR = t; }
void orders::setTS(int t) { TS = t; }
void orders::setTF(int t) { TF = t; }

int orders::getCookPeriod() const { return TR - TA; }
int orders::getServiceDuration() const { return TF - TS; }
int orders::getWaitTime() const {
    return (TA - TQ) + (TS - TR);
}

double orders::getPriority() const {
    if (type != TYPE_OVG) return 0;
    return (price * 0.5) / (size * 0.3 + distance * 0.2 + 1);
}

int orders::getID() const { return ID; }
ORD_TYPE orders::getType() const { return type; }
int orders::getTQ() const { return TQ; }
int orders::getSize() const { return size; }
double orders::getPrice() const { return price; }
int orders::getDistance() const { return distance; }
int orders::getNoOfSeats() const { return noOfSeats; }
int orders::getOrderDuration() const { return orderDuration; }

std::ostream& operator<<(std::ostream& os, const orders* ord) {
    if (ord) {
        os << ord->ID;
    }
    return os;
}
