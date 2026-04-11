#pragma once
#include "Actions/Actions.h"
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
	RequestAction(int id, ORD_TYPE type, int TQ, int size, int price,
		int seats = -1, int duration = -1, int distance = -1, bool sharable = false) ;

	virtual void ACT();
};

