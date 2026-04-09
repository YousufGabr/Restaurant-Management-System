#include "scooters.h"

scooters::scooters(int id, int speed, int Main_Dur)
{
}

int scooters::get_Speed() const
{
	return this->Speed;
}

int scooters::get_Maintenance_Duration() const
{
	return this->Maintenance_Duration;
}

int scooters::get_ID() const
{
	return this->ID;
}

void scooters::set_finish_time(int time)
{
	this->finish_time = time;
}

bool scooters::is_avilable(int currentTimestep) const
{
	return false;
}

bool scooters::assignOrder(const orders& OV)
{
	return false;
}

ostream& operator<<(ostream& os, const scooters* scooter)
{
	if (scooter)
	{
		os << scooter->ID;
	}
	return os;
}

