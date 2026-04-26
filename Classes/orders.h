#pragma once
#include <iostream>
using namespace std;

// Forward declaration
class Chefs;


enum ORD_TYPE {
    TYPE_ODG, TYPE_ODN, TYPE_OT, TYPE_OVC, TYPE_OVG, TYPE_OVN
};

class Orders {
protected:
    int ID;
    int size;
    double price;

    int TQ;
    int TA;
    int TR;
    int TF;

    ORD_TYPE type;

    Chefs* assignedChef;
    
public:
    Orders(ORD_TYPE type ,int tq, int id, int sz, double pr);

    int getID() const;
    int getTQ() const;
    int getSize() const;
    double getPrice() const;
    int getCookPeriod() const;
    
// Instant time steps 
    int getTA() const { return TA; }
    int getTR() const { return TR; } 
    int getTF() const { return TF; }

    void setTA(int t);
    void setTR(int t);
    void setTF(int t);

// Time periods
    
    int getTC() const; // setting cooking time period = TR-TA 
    virtual int getTW() const = 0;  // = TI + TC , diff implementation for OT
    virtual int getTi() const = 0; //idle time = (TA - TQ) + (TS - TR) , pure virual -> different implementation for OT
   
    void setAssignedChef(Chefs* chf);
    Chefs* getAssignedChef() const;

    // general for all orders 
    int getCookingpriority() const;

    ORD_TYPE getType() const;

    friend ostream& operator<<(ostream& os, const Orders* ord);
	virtual void print(ostream& os) const = 0;

    virtual ~Orders() {};

};
