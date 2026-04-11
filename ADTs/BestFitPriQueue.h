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
	int x = 1;
	while (dequeue(temp,x))
	{
		if (temp->get_free_Seats() >= OD->getNoOfSeats())
		{
			best = temp;
			break;
		}
		Qtemp.enqueue(temp);
	}

	while (dequeue(temp,x))
	{ 
		Qtemp.enqueue(temp);
	}

	while (Qtemp.dequeue(temp))
	{
		enqueue(temp,x);
	}
	temp = nullptr;
	return best;
}