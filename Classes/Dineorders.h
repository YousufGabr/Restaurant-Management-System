#pragma once
#include "orders.h"
#include "../Classes/chefs.h"
#include "../Classes/tables.h"

class Dineorders : public Orders
{
private:
    int noOfSeats;
    int orderDuration;
    bool canShare;
    int TS;
    
    Tables* AssignedTable;

public :
    Dineorders(ORD_TYPE type ,int tq, int id, int sz, double pr ,int seats, int duration, bool sharable);


    int getNoOfSeats() const;
    Tables* getAssignedTable() const;
    bool isSharable() const;
    void setAssignedTable(Tables* tbl);
    

    int getTS() const { return this->TS; }
    void setTS(int t);
    
    int getorderDuration() const;
    int getTserv() const; 
    virtual int getTi() const;
    virtual int getTW() const;

    int getServicePriority();

    virtual void print(ostream& os) const override;
   

};

