#pragma once
#include "PriorityQueue.h"
#include "LinkedQueue.h"
#include "../Classes/Deliveryorders.h"

class CancelPriQueue :public PriorityQueue<Orders *> //class specialization inherited from template class
{

public:

	Orders * CancelOrder(int ID)
	{
		PriorityQueue<Orders*> Qtemp;
		Orders* temp = nullptr;
		Orders* canceled = nullptr;
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