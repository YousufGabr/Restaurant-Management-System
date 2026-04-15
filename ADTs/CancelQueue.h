#pragma once
#include "../ADTs/LinkedQueue.h"
#include "../Classes/Orders.h"
class CancelQueue : public LinkedQueue<Orders*>
{
public :
	Orders* CancelOrder(int ID)
	{
		LinkedQueue<Orders*> Qtemp; // Qtemp should be priotized to store the values of prioritires
		Orders* temp = nullptr;
		Orders* canceled = nullptr;
		int pri = 0;

		while (dequeue(temp))
		{
			if (temp->getID() == ID && temp->getType() == TYPE_OVC)
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
};

