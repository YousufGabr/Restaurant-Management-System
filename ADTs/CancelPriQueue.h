#pragma once
#include "PriorityQueue.h"
#include "LinkedQueue.h"
#include "../Classes/Deliveryorders.h"

class CancelPriQueue :public PriorityQueue<Deliveryorders *> //class specialization inherited from template class
{

public:

	Deliveryorders * CancelOrder(int ID)
	{
		PriorityQueue<Deliveryorders*> Qtemp;
		Deliveryorders* temp = nullptr;
		Deliveryorders* canceled = nullptr;
		int pri = 0;

		while (dequeue(temp, pri))
		{
			if (temp->getID() == ID && temp->getType() == TYPE_OVC)
			{
				canceled = temp;
				continue;
			}
			Qtemp.enqueue(temp, pri);
		}
		while (Qtemp.dequeue(temp, pri))
		{
			enqueue(temp, pri);
		}
		temp = nullptr;
		return canceled;

	}
};