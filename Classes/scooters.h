#pragma once
#include <iostream>
#include <ostream>
#include "Orders.h"
using namespace std;

class Scooters
{
private:
	int ID;
	int Speed;
	int Maintenance_Duration;
	int Main_Ords_Threshold; // Added: to store limit before maintenance
	int orders_delivered_count; // Added: counter for assigned Orders
	int Tripdistance;
	int TotalDistance;
	int Tstart;
	int Tfinish;
	

public:
	Scooters(int id, int speed, int Main_Dur, int threshold); // Edited: added threshold parameter
	int get_Speed() const;
	int get_Maintenance_Duration() const;
	int getID() const;

	// set and get time steps for scooter per each trip
	int get_StartTime() const;
	int getfinish_time() const;
	void setfinish_time(int time);
	void setstart_time(int time);

    //set and get each trip distance 
	void setTripdistance(int distance);
	int  getTripdistance() const;

	//priorities
	int getFreePriority();
	int getBackPriority();

	int getTotalDistance();
	void updateCount();
	int getCount();
	void updateTotalDistance(int);
	int getSpeed() { return Speed; }
	int getMain_Ords_Threshold();
	void fix();

	


	friend ostream& operator<<(ostream& os, const Scooters* scooter);
};
