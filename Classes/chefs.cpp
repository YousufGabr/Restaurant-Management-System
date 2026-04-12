#include "Chefs.h"
using namespace std;
Chefs::Chefs(int id, CHFTYPE t, int spd)
    : ID(id), type(t), speed(spd), busyTime(0), finishTime(0) {
}

int Chefs::getID() const { return ID; }
Chefs::CHFTYPE Chefs::getType() const { return type; }
int Chefs::getSpeed() const { return speed; }

void Chefs::setFinishTime(int time) { finishTime = time; }

// Chef is available if the current timestep has reached or passed their finishTime
bool Chefs::isAvailable(int currentTimestep) const {
    return currentTimestep >= finishTime;
}

void Chefs::makeAvailable() { finishTime = 0; }

ostream& operator<<(std::ostream& os, const Chefs* pChf) {
    if (pChf) os << pChf->ID;
    return os;
}
