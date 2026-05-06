#pragma once
#include <iostream>
using namespace std;

// Forward declaration
class Chefs;


enum ORD_TYPE {
    TYPE_ODG, TYPE_ODN, TYPE_OT, TYPE_OVC, TYPE_OVG, TYPE_OVN , TYPE_COMBO
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
    Orders(ORD_TYPE type ,int Tq, int Id, int size, double price);

    int getID() const;
    int getSize() const;
    double getPrice() const;
    ORD_TYPE getType() const;
    
    // Instant time steps 
    int getTA() const { return TA; }
    int getTR() const { return TR; } 
    int getTF() const { return TF; }
    int getTQ() const { return TQ; }

    void setTA(int t);
    void setTR(int t);
    void setTF(int t);
    void setTQ(int t);
    
    // Time periods
    int getTC() const; // the exact cooking time 
    virtual int getTW() const = 0;
    virtual int getTi() const = 0; 
   
    void setAssignedChef(Chefs* chf);
    Chefs* getAssignedChef() const;

    // general for all orders 
   virtual int getCookingpriority() const;
   virtual int getCookingOrderDuration() const;

   
    friend ostream& operator<<(ostream& os, const Orders* ord);
	virtual void print(ostream& os) const = 0;

    virtual ~Orders() {};

};
