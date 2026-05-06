#pragma once
#include "orders.h"
#include"../Classes/scooters.h"
class ComboOrders : public Orders
{
private:
	Scooters** AssignedScooters; // will point to an array
	Chefs** Assignedchefs; // will point to array of assigned chefs
	int ScootersNumber;
	int chefsNumber;
	int CN;
	int CS;
	int TS;
	int distance;
	//holds average periods
	int averageCookingPeriod; 
	int averageDeliveryPeriod;
public:
	ComboOrders(int tq, int id, int sz, double pr, int distance , int Chefsumber , int cn , int cs , int scootersnumber);
	
	//combo setters
	void setCN(int cn);
	void setSN(int sn);
	void setAssignedScooters(Scooters**);
	void setAssignedChefs(Chefs** s);
	

	//combo getters 
	int getCN() const;
	int getCS() const;
	int getChefsNumber() const;
	int getScootersNumber() const;
	Scooters** getAssignedScooters()const;
	Chefs** getAssignedChefs() const;
	int getDistance () const { return distance; }
	void setAVGcooking(int, int);
	void setAVGscooters(int);



	//time instants and periods 
	void setTS(int time);
	int getTs() const;
	int getTserv() const; // Tserv = TF - TC 
	virtual int getTi() const;
	virtual int getTW() const;
	int getDeliveryDuration() const;

	
	virtual int getCookingOrderDuration() const override;
	void setDeliveryDistance(int dist);

	

   //priorities
	int getpendingPriority() const; //least numver of both chefs and scooters 
	int getServicePriority() const;
	virtual int getCookingpriority() const override;
	

	virtual void print(ostream& os) const override;
	
	

};

