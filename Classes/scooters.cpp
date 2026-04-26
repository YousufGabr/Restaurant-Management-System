#include "Scooters.h"

Scooters::Scooters(int id, int speed, int Main_Dur, int threshold)
	: ID(id), Speed(speed), Maintenance_Duration(Main_Dur), Main_Ords_Threshold(threshold),
	orders_delivered_count(0), Tfinish(0), TotalDistance(0) , Tstart(0) , Tripdistance(0)  // Edited: initialize members
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



void Scooters::setfinish_time(int time)
{
	this->Tfinish = time;
}

void Scooters::setstart_time(int time)
{
	this->Tstart = time;
}

int Scooters::get_StartTime() const
{
	return this->Tstart;
}

int Scooters::getfinish_time() const
{
	return this->Tfinish;
}



ostream& operator<<(ostream& os, const Scooters* scooter)
{
	if (scooter)
	{
		os << "S" << scooter->ID;
	}
	return os;
}

int Scooters :: getFreePriority()
{
	return ( -1 * TotalDistance);
}

// based on distance back = speed * back time 
int Scooters::getBackPriority()
{
	return -1 * getTripdistance();
}

int Scooters::getTotalDistance()
{
	return TotalDistance;
}

void Scooters::updateCount()
{
	this->orders_delivered_count ++;
}

int Scooters::getCount()
{
	return orders_delivered_count;
}

void Scooters::updateTotalDistance(int distance)
{
	this->TotalDistance += distance;
}

int Scooters::getMain_Ords_Threshold()
{
	return Main_Ords_Threshold;
}

void Scooters::fix()
{
	this->TotalDistance = 0;
	this->orders_delivered_count = 0;
}

void Scooters::setTripdistance(int distance)
{
	this->Tripdistance = distance;
}

int Scooters::getTripdistance() const
{
	return Tripdistance;
}
