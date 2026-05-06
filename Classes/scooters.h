#pragma once
#include <iostream>
#include <ostream>
#include "Orders.h"
using namespace std;

enum SC_TYPE {
	TYPE_NORMAL, TYPE_RESCUE
};

class Scooters
{
private:
	int ID;
	int Speed;
	int Maintenance_Duration;
	int Main_Ords_Threshold;
	int orders_delivered_count;
	int Tripdistance;
	int TotalDistance;
	int Tstart;
	int Tfinish;
    int maxTripsBeforeMaintenance;
    //added for rescue scooters
	bool isBroken;
	SC_TYPE Type;
	

public:
	Scooters(SC_TYPE type,int id, int speed, int Main_Dur, int threshold); // Edited: added threshold parameter
	SC_TYPE getType() const { return Type; }
	int get_Speed() const;
	int get_Maintenance_Duration() const;
	int getID() const;
	void set_Breakdown(bool b) { isBroken = b; }
	bool get_Breakdown() const { return isBroken; }

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

    //trips parameters
	int getTotalDistance();
	void updateTotalDistance(int);
	void updateCount();
	int getCount();
	
	
    //maintnance -> increase abbility to do more trips
	int get_MaxTripsBeforaMaint() const;
	void updateMaxTrips();
	void fix();

	


	friend ostream& operator<<(ostream& os, const Scooters* scooter);
};
