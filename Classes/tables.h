#pragma once
#include<iostream>
#include <ostream>
#include "orders.h"
using namespace std;

class tables
{
private:
	int ID;
	int Capacity;
	int free_Seats;
	int start_Service_time;
	int finish_time;
public:
	tables(int id, int capacity);
	int get_capacity();
	int get_free_Seats();

	void set_finish_time(int time);

	bool is_free(int currentTimestep);
	bool assign_order(const orders& OD);

	friend ostream& operator<<(ostream& os, const tables* table);

};

