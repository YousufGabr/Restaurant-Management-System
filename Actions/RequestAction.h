#pragma once
#include "Actions.h"
#include "../Classes/orders.h"
#include "../Restaurant/Restaurant.h"
#include "../Classes/Dineorders.h"
#include "../Classes/Takeawayorders.h"
#include "../Classes/Deliveryorders.h"

class RequestAction : public Actions
{
private :
	int ID;   
	int size;
	int price;
	int numberofseats;
	ORD_TYPE type;
	int Duration;
	int distance;
	bool canshare;
public:
	RequestAction(Restaurant* r ,int id, ORD_TYPE type, int TQ, int size, int price) ;

	RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price,
		int seats , int duration , bool sharable);

	RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price,int distance );

	virtual void ACT();
};

