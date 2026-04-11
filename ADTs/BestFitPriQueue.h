#pragma once
#include "PriorityQueue.h"
#include "LinkedQueue.h"
#include "../Classes/tables.h"
#include "../Classes/orders.h"
class BestFitPriQueue : public PriorityQueue<tables *>
{
	tables * getBest(orders * OD);
};

tables * BestFitPriQueue :: getBest (orders * OD)
{
	LinkedQueue<tables*> Qtemp;
	tables* temp = nullptr;
	tables* best = nullptr;

	while (dequeue(temp))
	{
		if (temp->get_free_Seats() >= OD->getNoOfSeats())
		{
			best = temp;
			break;
		}
		Qtemp.enqueue(temp);
	}

	while (dequeue(temp))
	{ 
		Qtemp.enqueue(temp);
	}

	while (Qtemp.dequeue(temp))
	{
		enqueue(temp);
	}
	temp = nullptr;
	return best;
}