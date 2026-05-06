#pragma once
#include <iostream>

class Chefs
{
public:
    enum CHFTYPE {
        TYPE_CN,
        TYPE_CS
    };

private:
    int ID;
    CHFTYPE type;
    int speed;
    int finishTime;

public:
    Chefs(int id, CHFTYPE t, int spd);

    int getID() const;
    CHFTYPE getType() const;
    int getSpeed() const;

    
    void setFinishTime(int time);


    friend std::ostream& operator<<(std::ostream& os, const Chefs* pChf);
};
