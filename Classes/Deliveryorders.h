#pragma once
#include "orders.h"
#include "../Classes/chefs.h"
#include "../Classes/Scooters.h"
class Deliveryorders : public Orders
{
private:
	int TH;
	int distance;
	int TS;
	bool isow;

	Scooters* AssignedScooter;
public:
	Deliveryorders(ORD_TYPE , int tq, int id, int sz, double pr, int distance, int TH);

	void setAssignedScooter(Scooters* sct);
	void setDeliveryDistance(int dist);
	int getDistance() const;
	Scooters* getAssignedScooter() const;
	

	void setTS(int t);
	int getTS() const { return TS; }
	int getTH() const { return TH; }

	int getTserv() const; // Tserv = TF - TC 
	virtual int getTi() const;
	virtual int getTW() const;
	int getDeliveryDuration() const;

	void setow(bool ow) { isow = ow; }
	bool getow() const { return isow; }


	int getOVGpriority() const;
	int getServicePriority() const;
	


	virtual void print(ostream& os) const override;
};

