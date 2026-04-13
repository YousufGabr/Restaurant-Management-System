#include "Scooters.h"

Scooters::Scooters(int id, int speed, int Main_Dur, int threshold)
	: ID(id), Speed(speed), Maintenance_Duration(Main_Dur), Main_Ords_Threshold(threshold),
	orders_delivered_count(0), finish_time(0), currentOrder(nullptr), TotalDistance(0) // Edited: initialize members
{
}

int Scooters::get_Speed() const
{
	return this->Speed;
}

int Scooters::get_Maintenance_Duration() const
{
	return this->Maintenance_Duration;
}

int Scooters::getID() const
{
	return this->ID;
}

void Scooters::set_finish_time(int time)
{
	this->finish_time = time;
}

int Scooters::get_finish_time() const
{
	return this->finish_time;
}

bool Scooters::is_available(int currentTimestep) const
{
	return (currentTimestep >= finish_time); // Edited: actual availability check
}

void Scooters::assignOrder(Orders* OV, int currentTimestep)
{
	currentOrder = OV; // Edited: store pointer
	int trip_time = OV->getDistance() / Speed; // Edited: calculate travel time
	OV->setTS(currentTimestep); // Edited: record Service Start Time
	OV->setTF(currentTimestep + trip_time); // Edited: record Finish Time (delivery)
	finish_time = currentTimestep + (2 * trip_time); // Edited: scooter returns after round trip
	orders_delivered_count++; // Edited: increment maintenance counter
	setTotalDistance(OV->getDistance());
}



ostream& operator<<(ostream& os, const Scooters* scooter)
{
	if (scooter)
	{
		os << "S" << scooter->ID;
	}
	return os;
}

int Scooters :: getPriority()
{
	return ( -1 * TotalDistance);
}

int Scooters::getTotalDistance()
{
	return TotalDistance;
}

void Scooters::setTotalDistance(int distance)
{
	this->TotalDistance += distance;
}
