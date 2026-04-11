#pragma once
#include "PriorityQueue.h"
#include "LinkedQueue.h"
#include "../Classes/orders.h"

class CancelPriQueue :public PriorityQueue<orders *> //class specialization inherited from template class
{

public:

	orders * CancelOrder(int ID);
};


orders* CancelPriQueue::CancelOrder(int ID)
{
	LinkedQueue<orders*> Qtemp;
	orders* temp = nullptr;
	orders* canceled = nullptr;

	while (dequeue(temp))
	{
		if (temp->getID() == ID)
		{
			canceled = temp;
			continue;
		}
		Qtemp.enqueue(temp);
	}
	while (Qtemp.dequeue(temp))
	{
		enqueue(temp);
	}
	temp = nullptr;
	return canceled;
	
}
