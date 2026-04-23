#include "Orders.h"
#include "Chefs.h"

Orders::Orders(ORD_TYPE type ,int tq, int id, int sz, double pr) : type (type) ,ID(id), TQ(tq), size(sz), price(pr)
, TA(0), TR(0) , TF(0) , assignedChef(nullptr)
{
}
 
int Orders::getSize() const { return size; }
double Orders::getPrice() const { return price; }

void Orders::setAssignedChef(Chefs* chf) { assignedChef = chf; }
Chefs* Orders::getAssignedChef() const { return assignedChef; }


//instant timed
void Orders::setTA(int t) { TA = t; }
void Orders::setTR(int t) { TR = t; }
void Orders::setTF(int t) { TF = t; }


int Orders::getTC() const { return TR - TA; }
int Orders::getID() const { return ID; }
int Orders::getTQ() const { return TQ; }

//time periods
int Orders::getCookPeriod() const
{
	return TR - TA;
}

ORD_TYPE Orders::getType() const
{
    return this->type;
}

//priority based on duration of cooking ( t = size / assigned Chef speed )
int Orders::getCookingpriority() const
{
	return (size / (getAssignedChef()->getSpeed())) * -1;
}






