#pragma once
#include "PriorityQueue.h"
#include "LinkedQueue.h"
#include "../Classes/Tables.h"
#include "../Classes/Orders.h"
class BestFitPriQueue : public PriorityQueue<Tables*>
{
	Tables* getBest(Orders* OD);
};

Tables* BestFitPriQueue::getBest(Orders* OD)
{
	PriorityQueue<Tables*> Qtemp; // to store the priority value
	Tables* temp = nullptr;
	Tables* best = nullptr;
	int PRI = 0;
	while (dequeue(temp,PRI))
	{
		if (temp->get_free_Seats() >= OD->getNoOfSeats())
		{
			best = temp;
			break;
		}
		Qtemp.enqueue(temp , PRI);
	}

	while (dequeue(temp,PRI))
	{ 
		Qtemp.enqueue(temp , PRI);
	}

	while (Qtemp.dequeue(temp , PRI))
	{
		enqueue(temp,PRI);
	}
	temp = nullptr;
	return best;
}