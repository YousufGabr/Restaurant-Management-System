#include "ComboOrders.h"

ComboOrders::ComboOrders(int tq, int id, int sz, double pr, int distance, int Chefsumber, int cn, int cs, int scootersnumber) : Orders(TYPE_COMBO, tq, id, sz, pr),
AssignedScooters(nullptr), ScootersNumber(scootersnumber), chefsNumber(Chefsumber), distance(distance) , CN(cn) , CS(cs), TS(0) , Assignedchefs(nullptr)
{

}

void ComboOrders::setCN(int cn)
{
	this->chefsNumber = cn;
}

void ComboOrders::setSN(int sn)
{
	this->ScootersNumber = sn;
}

void ComboOrders::setAssignedScooters(Scooters** s)
{
	AssignedScooters = s;
}

void ComboOrders::setAssignedChefs(Chefs** s)
{
	Assignedchefs = s;
}

int ComboOrders::getChefsNumber() const
{
	return chefsNumber;
}

int ComboOrders::getScootersNumber() const
{
	return ScootersNumber;

}

int ComboOrders::getCS() const
{
	return CS;

}

int ComboOrders::getCN() const
{
	return CN;

}





Scooters** ComboOrders::getAssignedScooters() const
{
	return AssignedScooters;
}

Chefs** ComboOrders::getAssignedChefs() const
{
	return Assignedchefs;
}

void ComboOrders::setAVGcooking(int n, int s)
{
	if(CN == 0) this->averageCookingPeriod = (size) / ((s / CS));
	this->averageCookingPeriod = (size) / ((n / CN) + (s / CS));
}

void ComboOrders::setAVGscooters(int s)
{
	averageDeliveryPeriod = distance / (getScootersNumber() * s);
}

void ComboOrders::setDeliveryDistance(int dist)
{
	this->distance = dist;
}
int ComboOrders::getpendingPriority() const
{
	return -1*(chefsNumber + ScootersNumber);
}

int ComboOrders::getServicePriority() const
{
	return -1* averageDeliveryPeriod;
}



void ComboOrders::setTS(int t)
{
	this->TS = t;
}

int ComboOrders::getTs() const
{
	return TS;
}


int ComboOrders::getTserv() const { return TF - TS; }

int ComboOrders::getTW() const {
	return getTi() + getTC();
}
int ComboOrders::getTi() const
{
	return  (TA - TQ) + (TS - TR);
}


int ComboOrders::getDeliveryDuration() const
{
	return averageDeliveryPeriod;
}

int ComboOrders::getCookingpriority() const
{
	return -1 * averageCookingPeriod;
}

int ComboOrders::getCookingOrderDuration() const
{
	
	return averageCookingPeriod;
}


void ComboOrders::print(ostream& os) const
{
	if (getAssignedScooters() != nullptr)
	{

		os << "[" << getID() << ",S=" << getScootersNumber() << "]";
	}
	else if (getAssignedChefs() != nullptr)
	{
		os << "[" << getID() << ",CN=" << getCN() << ",CS=" <<getCS() << "]";
	}
	else os << getID();
}










