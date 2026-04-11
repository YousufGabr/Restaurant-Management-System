#pragma once
#include "PriorityQueue.h"
#include "LinkedQueue.h"
#include "../Classes/Orders.h"

class CancelPriQueue :public PriorityQueue<Orders *> //class specialization inherited from template class
{

public:

	Orders * CancelOrder(int ID);
};


Orders* CancelPriQueue::CancelOrder(int ID)
{
	LinkedQueue<Orders*> Qtemp;
	Orders* temp = nullptr;
	Orders* canceled = nullptr;
	int x = 1;

	while (dequeue(temp,x))
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
		enqueue(temp,x);
	}
	temp = nullptr;
	return canceled;
	
}
