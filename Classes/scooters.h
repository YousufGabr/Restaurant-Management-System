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
	int finish_time;
	int TotalDistance;
	Orders* currentOrder; // Edited: changed to pointer to follow "Don't Copy" rule

public:
	Scooters(int id, int speed, int Main_Dur, int threshold); // Edited: added threshold parameter
	int get_Speed() const;
	int get_Maintenance_Duration() const;
	int get_ID() const;

	void set_finish_time(int time);
	int get_finish_time() const; // Added: to check when it returns

	bool is_available(int currentTimestep) const; // Edited: logic to check finish_time
	void assignOrder(Orders* OV, int currentTimestep); // Edited: changed to pointer and added logic
	int getPriority();
	int getTotalDistance();
	void setTotalDistance(int);

	friend ostream& operator<<(ostream& os, const Scooters* scooter);
};
