#pragma once
#include "orders.h"
#include "chefs.h"
class Takeawayorders : public Orders
{
public:

	Takeawayorders(int tq, int id, int sz, double pr);

	virtual int getTi() const;
	virtual int getTW() const;

	friend std::ostream& operator<<(std::ostream& os, const Takeawayorders* OD);
};

