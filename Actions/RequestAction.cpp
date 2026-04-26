#include "RequestAction.h"


RequestAction::RequestAction(Restaurant * r,int id, ORD_TYPE type,int TQ, int size, int price) : 
	numberofseats(0) , Duration(0) , canshare(false) , distance(-1)
{
	this->RestPtr = r;
	this->ID = id;
	this->type = type;
	this->currentTimestep = TQ;
	this->size = size;
	this->price = price;
	
}

RequestAction::RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price, int seats, int duration, bool sharable) :
	distance(-1)
{
	this->RestPtr = r;
	this->ID = id;
	this->type = type;
	this->currentTimestep = TQ;
	this->size = size;
	this->price = price;
	this->numberofseats = seats;
	this->Duration = duration;
	this->canshare = sharable;
}

RequestAction::RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price , int distance):
	numberofseats(0), Duration(0), canshare(false)
{
	this->RestPtr = r;
	this->ID = id;
	this->type = type;
	this->currentTimestep = TQ;
	this->size = size;
	this->price = price;
	this->distance = distance;

}

// 1-Create new order according to its type 2-Add it to pending lists by calling the function in restaurant 
void RequestAction::ACT()
{
	Orders* neworder = nullptr; //every order is pointed to by the base class ptr 
	if (type == TYPE_ODG || type == TYPE_ODN)
	{
		neworder = new Dineorders(type ,currentTimestep, ID, size ,price,
			numberofseats, Duration, canshare);
    }
	else if (type == TYPE_OT)
	{
		neworder = new Takeawayorders(currentTimestep, ID, size, price);
	}
	else
	{
		neworder = new Deliveryorders(type, currentTimestep, ID, size, price, distance);
	}

	RestPtr->AddPendingOrder(neworder);
}

void RequestAction::print(ostream& os) const
{
	string typeStr;
	switch (type) {
		case TYPE_ODG: typeStr = "ODG"; break;
		case TYPE_ODN: typeStr = "ODN"; break;
		case TYPE_OT:  typeStr = "OT";  break;
		case TYPE_OVC:  typeStr = "OVC";  break;
		case TYPE_OVG:  typeStr = "OVG";  break;
		case TYPE_OVN:  typeStr = "OVN";  break;
	}
    os << "[" << typeStr << "," << currentTimestep << "," << ID << "] ";
}


