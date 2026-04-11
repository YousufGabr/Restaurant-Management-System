#include "Chefs.h"
#include "orders.h"

Chefs::Chefs(int id, CHFTYPE t, int spd)
    : ID(id), type(t), speed(spd), busyTime(0), currentOrd(nullptr), finishTime(0) {
}

int Chefs::getID() const { return ID; }

Chefs::CHFTYPE Chefs::getType() const { return type; }

ostream& operator<<(ostream& os, const Chefs* pChf) {
    if (pChf) {
        os << pChf->ID;
    }
    return os;
}
