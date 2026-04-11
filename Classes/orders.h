#pragma once
#include <iostream>

enum ORD_TYPE {
    TYPE_ODG,
    TYPE_ODN,
    TYPE_OT,
    TYPE_OVC,
    TYPE_OVG,
    TYPE_OVN
};

class orders {
private:
    int ID;
    ORD_TYPE type;
    int TQ;
    int size;
    double price;

    int noOfSeats;
    int orderDuration;
    bool canShare;

    int distance;

    int TA;
    int TR;
    int TS;
    int TF;

public:
    orders(int id, ORD_TYPE t, int tq, int sz, double pr);

    int getID() const;
    ORD_TYPE getType() const;
    int getTQ() const;
    int getSize() const;
    double getPrice() const;

    void setDineInInfo(int seats, int duration, bool share);
    void setDeliveryDistance(int dist);
    int getDistance() const;
    int getNoOfSeats() const;
    int getOrderDuration() const;

    void setTA(int t);
    void setTR(int t);
    void setTS(int t);
    void setTF(int t);

    int getCookPeriod() const;
    int getWaitTime() const;
    int getServiceDuration() const;

    double getPriority() const;

    friend std::ostream& operator<<(std::ostream& os, const orders* ord);
};
