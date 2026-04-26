#pragma once
#include "../ADTs/LinkedQueue.h"
#include "../Classes/Deliveryorders.h"
class CancelQueue : public LinkedQueue<Orders*>
{
public :
	Orders* CancelOrder(int ID)
	{
		LinkedQueue<Orders*> Qtemp;
		Orders* temp = nullptr;
		Orders* canceled = nullptr;

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

