#pragma once
#include "Orders.h"
#include <iostream>
#include <ostream>
using namespace std;
class Chefs
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
    Orders* currentOrd; 
    int finishTime; 

public:
    Chefs(int id, CHFTYPE t, int spd);

    int getID() const;
    CHFTYPE getType() const;
    int getSpeed() const;

    void assignOrder(Orders* ord, int currentTimestep);
    bool isAvailable(int currentTimestep) const;

    friend ostream& operator<<(ostream& os, const Chefs* pChf);




};

