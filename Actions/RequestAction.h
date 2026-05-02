#pragma once
#include "Actions.h"
#include "../Classes/orders.h"
#include "../Restaurant/Restaurant.h"
#include "../Classes/Dineorders.h"
#include "../Classes/Takeawayorders.h"
#include "../Classes/Deliveryorders.h"
#include "../Classes/chefs.h"


class RequestAction : public Actions
{
private :   
	int size;
	int price;
	int numberofseats;
	ORD_TYPE type;
	int Duration;
	int distance;
	bool canshare;
	int TH;
	//Combo orders
	int ChefsNumber;
	int CN;
	int CS;
	int ScootersNumber;
	
public:
	RequestAction(Restaurant* r ,int id, ORD_TYPE type, int TQ, int size, int price) ;

	RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price,
		int seats , int duration , bool sharable);

	RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price,int distance , int TH = 0);

	RequestAction(Restaurant* r, int id, ORD_TYPE type, int TQ, int size, int price, int distance, int ChefsNumber ,int cn ,int cs,int ScootersNumber);


	virtual void ACT();

	virtual void print(ostream& os) const override;
};

