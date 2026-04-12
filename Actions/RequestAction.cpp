#include "RequestAction.h"

RequestAction::RequestAction(Restaurant * r,int id, ORD_TYPE type, int TQ, int size, int price, int seats, int duration, int distance, bool sharable)
{
	this->RestPtr = r;
	this->ID = id;
	this->type = type;
	this->currentTimestep = TQ;
	this->size = size;
	this->price = price;
	this->numberofseats = seats;
	this->Duration = duration;
	this->distance = distance;
	this->canshare = sharable;
}

void RequestAction::ACT()
{
	Orders* newOrder = new Orders(ID, type, currentTimestep, size, price);
	if (type == TYPE_ODG || type == TYPE_ODN)
	{
		 
		newOrder->setDineInInfo(numberofseats, Duration, canshare);
		//RestPtr->AddOrder(newOrder); 
	}
	else if (type == TYPE_OVG || type == TYPE_OVN || type == TYPE_OVC)
	{
		newOrder->setDeliveryDistance(distance);
		//RestPtr->AddOrder(newOrder);
	}
	else
	{
		//RestPtr->AddOrder(newOrder);
	}


}
