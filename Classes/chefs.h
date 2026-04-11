#pragma once
#include "orders.h"
#include <iostream>
#include <ostream>
using namespace std;
class chefs
{

public:
    enum CHFTYPE {
        TYPE_CN,
        TYPE_CS
    };

private :
    int ID;
    CHFTYPE type;
    int speed;         
    int busyTime;    
    orders* currentOrd; 
    int finishTime; 

public:
    chefs(int id, CHFTYPE t, int spd);

    int getID() const;
    CHFTYPE getType() const;
    int getSpeed() const;

    void assignOrder(orders* ord, int currentTimestep);
    bool isAvailable(int currentTimestep) const;

    friend ostream& operator<<(ostream& os, const chefs* pChf);




};

