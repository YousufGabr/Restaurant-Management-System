#include "chefs.h"
#include "orders.h"

chefs::chefs(int id, CHFTYPE t, int spd)
    : ID(id), type(t), speed(spd), busyTime(0), currentOrd(nullptr), finishTime(0) {
}

int chefs::getID() const { return ID; }

chefs::CHFTYPE chefs::getType() const { return type; }

ostream& operator<<(ostream& os, const chefs* pChf) {
    if (pChf) {
        os << pChf->ID;
    }
    return os;
}
