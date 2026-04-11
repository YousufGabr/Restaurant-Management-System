#include "tables.h"

tables::tables(int id, int capacity)
	: ID(id), Capacity(capacity), free_Seats(capacity), finish_time(0), currentOrder(nullptr) // Edited: initialize members
{
}

int tables::get_capacity() const
{
	return this->Capacity;
}

int tables::get_free_Seats() const
{
	return this->free_Seats;
}

void tables::set_finish_time(int time)
{
	this->finish_time = time;
}

int tables::get_finish_time() const
{
	return this->finish_time;
}

bool tables::is_free(int currentTimestep) const
{
	return (currentTimestep >= finish_time); // Edited: actual free check
}

void tables::assign_order(orders* OD, int currentTimestep)
{
	currentOrder = OD; // Edited: store pointer
	OD->setTS(currentTimestep); // Edited: record Service Start Time
	finish_time = currentTimestep + OD->getOrderDuration(); // Edited: set when table becomes free
	OD->setTF(finish_time); // Edited: record Finish Time
	free_Seats -= OD->getNoOfSeats(); // Edited: update seats for sharing logic
}

ostream& operator<<(ostream& os, const tables* table)
{
	if (table)
	{
		os << "T" << table->ID << "[" << table->Capacity << "," << table->free_Seats << "]"; // Edited: formatted output
	}
	return os;
}
