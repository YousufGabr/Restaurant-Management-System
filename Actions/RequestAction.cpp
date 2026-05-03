#include "RequestAction.h"


RequestAction::RequestAction(Restaurant * r,int id, ORD_TYPE type,int TQ, int size, int price) : 
	numberofseats(0) , Duration(0) , canshare(false) , distance(-1) , TH(0), CN(0),CS(0), ChefsNumber(0), ScootersNumber(0)
{
	this->RestPtr = r;
	this->ID = id;
	this->type = type;
	this->currentTimestep = TQ;
	this->size = size;
	this->price = price;
	
}

RequestAction::RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price, int seats, int duration, bool sharable) :
	distance(-1) , TH(0) , CN(0), CS(0) ,ChefsNumber(0),ScootersNumber(0)
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

RequestAction::RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price , int distance, int TH):
	numberofseats(0), Duration(0), canshare(false), CN(0), CS(0), ChefsNumber(0), ScootersNumber(0)
{
	this->RestPtr = r;
	this->ID = id;
	this->type = type;
	this->currentTimestep = TQ;
	this->size = size;
	this->price = price;
	this->distance = distance;
	this->TH = TH;
}

RequestAction::RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price, int distance, int ChefsNumber, int cn, int cs, int ScootersNumber):
	numberofseats(0), Duration(0), canshare(false) , TH(0)
{
	this->RestPtr = r;
	this->ID = id;
	this->type = type;
	this->currentTimestep = TQ;
	this->size = size;
	this->price = price;
	this->distance = distance;
	this->ChefsNumber = ChefsNumber;
	this->CN = cn;
	this->CS = cs;
	this->ScootersNumber = ScootersNumber;

}

// 1-Create new order according to its type 2-Add it to pending lists by calling the function in restaurant 
void RequestAction::ACT()
{
	Orders* neworder = nullptr; //every order is pointed to by the base class ptr 
	if (type == TYPE_COMBO)
	{
		neworder = new ComboOrders(currentTimestep , ID , size , price ,distance , ChefsNumber, CN,  CS,  ScootersNumber);
	}
	else if (type == TYPE_ODG || type == TYPE_ODN)
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
		neworder = new Deliveryorders(type, currentTimestep, ID, size, price, distance, TH);
	}

	RestPtr->AddPendingOrder(neworder , currentTimestep);
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
		case TYPE_COMBO:  typeStr = "COMBO";  break;
	}
    os << "[" << typeStr << "," << currentTimestep << "," << ID << "] ";
}


