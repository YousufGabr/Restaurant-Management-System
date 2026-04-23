#pragma once
#include "../ADTs/LinkedQueue.h"
#include "../Classes/Deliveryorders.h"
class CancelQueue : public LinkedQueue<Deliveryorders*>
{
public :
	Deliveryorders* CancelOrder(int ID)
	{
		LinkedQueue<Deliveryorders*> Qtemp;
		Deliveryorders* temp = nullptr;
		Deliveryorders* canceled = nullptr;

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

