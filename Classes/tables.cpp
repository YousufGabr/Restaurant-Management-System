#include "tables.h"
tables::tables(int id, int capacity)
{
}

int tables::get_capacity()
{
	return this->Capacity;
}

int tables::get_free_Seats()
{
	return this->free_Seats;
}

void tables::set_finish_time(int time)
{
	this->finish_time = time;
}

bool tables::is_free(int currentTimestep)
{
	return false;
}

bool tables::assign_order(const orders& OD)
{
	return false;
}

ostream& operator<<(ostream& os, const tables* table)
{
	if (table)
	{
		os << table->ID;
	}
    return os;
}
