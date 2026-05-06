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

public:
	Tables(int id, int capacity);
	int getID() const;
	int get_capacity() const;
	int get_free_Seats() const;
	void set_free_Seats(int seats);

	void set_finish_time(int time);
	int get_finish_time() const;

	bool is_free() const; 
	int getPriority();

	friend ostream& operator<<(ostream& os, const Tables* table);
};
