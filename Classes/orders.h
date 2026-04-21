#pragma once
#include <iostream>

// Forward declaration
class Chefs;
class Scooters;
class Tables;

enum ORD_TYPE {
    TYPE_ODG, TYPE_ODN, TYPE_OT, TYPE_OVC, TYPE_OVG, TYPE_OVN
};

class Orders {
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

    
    Chefs* assignedChef;
    Scooters* AssignedScooter;
    Tables* AssignedTable;

public:
    Orders(ORD_TYPE t, int tq, int id, int sz, double pr);

    int getID() const;
    ORD_TYPE getType() const;
    int getTQ() const;
    int getSize() const;
    double getPrice() const;
    bool isSharable() const;

    void setDineInInfo(int seats, int duration, bool share);
    void setDeliveryDistance(int dist);
    int getDistance() const;
    int getNoOfSeats() const;
    int getOrderDuration() const;

    void setTA(int t);
    void setTR(int t);
    void setTS(int t);
    void setTF(int t);

    
    void setAssignedChef(Chefs* chf);
    Chefs* getAssignedChef() const;
    
    void setAssignedScooter(Scooters* sct);
    Scooters* getAssignedScooter() const;

	void setAssignedTable(Tables* tbl);
	Tables* getAssignedTable() const;

    int getCookPeriod() const;
    int getWaitTime() const;
    int getServiceDuration() const;

    int getPriority() const;

    friend std::ostream& operator<<(std::ostream& os, const Orders* ord);
int getTA() const { return TA; }
int getTR() const { return TR; }
int getTS() const { return TS; }
int getTF() const { return TF; }
};
