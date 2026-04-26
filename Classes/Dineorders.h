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
    int getTS() const { return this->TS;}


    void setAssignedTable(Tables* tbl);
    void setNoOfSeats(int seats);
    void setTS(int t);
    void setorderDuration(int t);

    int getTserv() const; // = TF-TS
    virtual int getTi() const;
    virtual int getTW() const;

    int getServicePriority();

    virtual void print(ostream& os) const override;
   

};

