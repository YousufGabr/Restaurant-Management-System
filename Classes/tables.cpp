#include "Tables.h"

Tables::Tables(int id, int capacity)
	: ID(id), Capacity(capacity), free_Seats(capacity), finish_time(0), currentOrder(nullptr) // Edited: initialize members
{
}

int Tables::getID() const { return ID; }

int Tables::get_capacity() const
{
	return this->Capacity;
}

int Tables::get_free_Seats() const
{
	return this->free_Seats;
}

void Tables::set_free_Seats(int seats)
{
	this->free_Seats = seats;
}

void Tables::set_finish_time(int time)
{
	this->finish_time = time;
}

int Tables::get_finish_time() const
{
	return this->finish_time;
}

bool Tables::is_free() const
{
	return (free_Seats == Capacity);
}

int Tables::getPriority()
{
	return -1 * free_Seats;
}

ostream& operator<<(ostream& os, const Tables* table)
{
	if (table)
	{
		os << "[" << "T" << table->ID << "," << table->Capacity << "," << table->free_Seats << "]"; // Edited: formatted output
	}
	return os;
}
