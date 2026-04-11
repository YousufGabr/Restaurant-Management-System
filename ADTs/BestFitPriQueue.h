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
	LinkedQueue<Tables*> Qtemp;
	Tables* temp = nullptr;
	Tables* best = nullptr;

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