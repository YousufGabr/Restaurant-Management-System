#pragma once

#include <iostream>
#include <ostream>
#include "orders.h"
using namespace std;

class scooters
{
private :
	int ID;
	int Speed;
	int Maintenance_Duration;
	int start_Service_time;
	int finish_time;

public :
	scooters(int id, int speed, int Main_Dur);
	int get_Speed() const;
	int get_Maintenance_Duration() const;
	int get_ID() const;

	void set_finish_time(int time);

	bool is_avilable(int currentTimestep) const;
	bool assignOrder(const orders& OV);

	friend ostream& operator<<(ostream& os, const scooters* table);

	

};

