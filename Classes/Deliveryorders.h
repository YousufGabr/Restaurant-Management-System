#pragma once
#include "orders.h"
#include "../Classes/chefs.h"
#include "../Classes/Scooters.h"
class Deliveryorders : public Orders
{
private:
	int distance;
	int TS;
	int TH;

	Scooters* AssignedScooter;
public:
	Deliveryorders(ORD_TYPE , int tq, int id, int sz, double pr, int distance, int TH);

	void setAssignedScooter(Scooters* sct);
	void setDeliveryDistance(int dist);
	void setTS(int t);


	int getDistance() const;
	Scooters* getAssignedScooter() const;
	int getTS() const { return TS; }
	int getTH() const { return TH; }

	int getTserv() const; // Tserv = TF - TC 
	virtual int getTi() const;
	virtual int getTW() const;


	int getOVGpriority() const;
	int getServicePriority() const;
	int getDeliveryDuration() const;


	virtual void print(ostream& os) const override;
};

