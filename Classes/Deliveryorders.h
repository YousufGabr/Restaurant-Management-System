#pragma once
#include "orders.h"
#include "../Classes/chefs.h"
#include "../Classes/Scooters.h"
class Deliveryorders : public Orders
{
private:
	int distance;
	int TS;

	Scooters* AssignedScooter;
public:
	Deliveryorders(ORD_TYPE , int tq, int id, int sz, double pr, int distance);

	void setAssignedScooter(Scooters* sct);
	void setDeliveryDistance(int dist);
	void setTS(int t);


	int getDistance() const;
	Scooters* getAssignedScooter() const;
	int getTS() const { return TS; }

	int getTserv() const; // Tserv = TF - TC 
	virtual int getTi() const;
	virtual int getTW() const;


	int getOVGpriority() const;
	int getServicePriority() const;


	friend std::ostream& operator<<(std::ostream& os, const Deliveryorders* OV);
};

