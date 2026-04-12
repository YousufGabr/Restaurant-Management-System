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
    int busyTime;
    int finishTime;  // Timestep when the chef will finish the current order

public:
    Chefs(int id, CHFTYPE t, int spd);

    int getID() const;
    CHFTYPE getType() const;
    int getSpeed() const;

    // --- Added to track when the chef becomes free ---
    void setFinishTime(int time);
    bool isAvailable(int currentTimestep) const;
    void makeAvailable(); // Used for cancellation

    friend std::ostream& operator<<(std::ostream& os, const Chefs* pChf);
};
