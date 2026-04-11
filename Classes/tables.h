#pragma once
#include <iostream>
#include <ostream>
#include "Orders.h"
using namespace std;

class Tables
{
private:
	int ID;
	int Capacity;
	int free_Seats;
	int finish_time;
	Orders* currentOrder; // Added: pointer to assigned order

public:
	Tables(int id, int capacity);
	int get_capacity() const;
	int get_free_Seats() const;

	void set_finish_time(int time);
	int get_finish_time() const;

	bool is_free(int currentTimestep) const; // Edited: check if finish_time passed
	void assign_order(Orders* OD, int currentTimestep); // Edited: changed to pointer and added logic

	friend ostream& operator<<(ostream& os, const Tables* table);
};
