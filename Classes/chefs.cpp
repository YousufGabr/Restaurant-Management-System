#include "Chefs.h"
using namespace std;
Chefs::Chefs(int id, CHFTYPE t, int spd)
    : ID(id), type(t), speed(spd), finishTime(0) {
}

int Chefs::getID() const { return ID; }
Chefs::CHFTYPE Chefs::getType() const { return type; }
int Chefs::getSpeed() const { return speed; }


void Chefs::setFinishTime(int time) { finishTime = time; }

ostream& operator<<(std::ostream& os, const Chefs* pChf) {
    if (pChf) os << pChf->ID;
    return os;
}
