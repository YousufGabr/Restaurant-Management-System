#pragma once
#include "orders.h"
#include "chefs.h"
class Takeawayorders : public Orders
{
public:

	Takeawayorders(int tq, int id, int sz, double pr);

	virtual int getTi() const;
	virtual int getTW() const;

	virtual void print(ostream& os) const override;
};

