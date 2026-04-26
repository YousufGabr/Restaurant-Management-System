#include "Restaurant.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Restaurant::Restaurant()
{
    
}



////////////////////////  ADD and Cancel Functions //////////////////////

void Restaurant::AddPendingOrder(Orders* neworder)
{
    ORD_TYPE orderType = neworder->getType();
    if (dynamic_cast<Deliveryorders*>(neworder))
    {
        Deliveryorders* newOV = dynamic_cast<Deliveryorders*>(neworder);
        switch (orderType)
        {
        case TYPE_OVN: PEND_OVN.enqueue(newOV); break;
        case TYPE_OVC: PEND_OVC.enqueue(newOV); break;
        case TYPE_OVG: PEND_OVG.enqueue(newOV, (newOV->getOVGpriority() * 1000)); break;
        }

    }
    else if (dynamic_cast<Dineorders*>(neworder))
    {
        Dineorders* newOD = dynamic_cast<Dineorders*>(neworder);
        switch (orderType)
        {
        case TYPE_ODG: PEND_ODG.enqueue(newOD); break;
        case TYPE_ODN: PEND_ODN.enqueue(newOD); break;
        }

    }
    else if (dynamic_cast<Deliveryorders*>(neworder))
    {
        Takeawayorders* newOT = dynamic_cast<Takeawayorders*>(neworder);
        PEND_OT.enqueue(newOT);
    }
}

void Restaurant::CancelOVC(int orderID)
{
    Orders* canceled = PEND_OVC.CancelOrder(orderID);
    if (canceled) { Canceled_Orders.push(canceled); return; }

    canceled = READY_OV.CancelOrder(orderID);
    if (canceled) { Canceled_Orders.push(canceled); return; }

    LinkedQueue<Orders*> tempQ;

    canceled = Cooking_Orders.CancelOrder(orderID);
    if (canceled)
    {

        Chefs* chf = canceled->getAssignedChef();
        if (chf)
        {
            if (chf->getType() == Chefs::TYPE_CS) Free_CS.enqueue(chf);
            else Free_CN.enqueue(chf);
            canceled->setAssignedChef(nullptr);
        }
        Canceled_Orders.push(canceled);
    }
    
}

////////////////////////////////////  Input and output functions ///////////////////////////



void Restaurant::loadInputFile()
{
    Actions* act = nullptr;
    // Note: You passed 'filename' as a parameter, but used 'ui.getinputfilename()'
    // I will use 'filename' for consistency.
    string filename = ui.getinputfilename();
    ifstream input(filename);

    if (!input.is_open()) {
        cout << "Error file cannot open\n";
        return;
    }

    // 1. Declare all variables ONCE at the top
    int numCN, numCS, speedCN, speedCS;
    int sCount, sSpeed, mainOrds, mainDur;
    int totalTables;
    int tablecount, capacity;
    int TH;
    int M;

    char Acttype = 0;
    string ordtype;
    int TQ, Tcancel, ID, size, price, numberofseats, Duration, distance;
    bool canshare;

    // 2. Reading Chef Data
    input >> numCN >> numCS >> speedCN >> speedCS;
    for (int i = 0; i < numCN; i++)
        Free_CN.enqueue(new Chefs(i + 1, Chefs::TYPE_CN, speedCN));
    for (int i = 0; i < numCS; i++)
        Free_CS.enqueue(new Chefs(numCN + i + 1, Chefs::TYPE_CS, speedCS));

    
    input >> sCount >> sSpeed >> mainOrds >> mainDur;
    for (int i = 0; i < sCount; i++) {
        Scooters* s = new Scooters(i + 1, sSpeed, mainOrds, mainDur);
        Free_Scooters.enqueue(s, s->getFreePriority());
    }

    
    input >> totalTables;
    for (int i = 0; i < totalTables; /* incrementing handled inside */) {
        input >> tablecount >> capacity;
        for (int j = 0; j < tablecount; j++) {
            Tables* newTable = new Tables(++i, capacity);
            Free_Tables.enqueue(newTable, capacity);
        }
    }

	input >> TH; // overwait threshold
    input >> M; // number of action lines

    //Reading actions and load them to lists
    for(int i =0 ; i<M; i++)
    {
        act = nullptr; // Reset act pointer each iteration
        input >> Acttype;

        if (Acttype == 'Q')
        {
            input >> ordtype;
            char share;

            if (ordtype == "ODG") {
                input >> TQ >> ID >> size >> price >> numberofseats >> Duration >> share;
                if(share == 'Y' || share == 'y') canshare = true;
				else canshare = false;
                act = new RequestAction(this, ID, TYPE_ODG, TQ, size, price, numberofseats, Duration, canshare);
            }
            else if (ordtype == "ODN") {
                input >> TQ >> ID >> size >> price >> numberofseats >> Duration >> share;
                if (share == 'Y' || share == 'y') canshare = true;
                else canshare = false;
                act = new RequestAction(this, ID, TYPE_ODN, TQ, size, price, numberofseats, Duration, canshare);
            }
            else if (ordtype == "OT") {
                input >> TQ >> ID >> size >> price;
                act = new RequestAction(this, ID, TYPE_OT, TQ, size, price);
            }
            else if (ordtype == "OVN") {
                input >> TQ >> ID >> size >> price >> distance;
                act = new RequestAction(this, ID, TYPE_OVN, TQ, size, price, distance);
            }
            else if (ordtype == "OVG") {
                input >> TQ >> ID >> size >> price >> distance;
                act = new RequestAction(this, ID, TYPE_OVG, TQ, size, price, distance);
            }
            else if (ordtype == "OVC") { // Fixed duplicate "OVN" check to "OVC"
                input >> TQ >> ID >> size >> price >> distance;
                act = new RequestAction(this, ID, TYPE_OVC, TQ, size, price, distance);
            }

            if (act) Request.enqueue(act);
        }
        else if (Acttype == 'X')
        {
            input >> Tcancel >> ID;
            act = new CancelAction(this, Tcancel, ID);
            if (act) Cancel.enqueue(act);
        }
    }

    input.close();
}

void Restaurant::generateOutputFile() {
	string filename = ui.getoutputfilename();
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not create output file!" << endl;
        return;
    }

    outFile << "TF\tID\tTQ\tTA\tTR\tTS\tTc\tTw\tTserv\n";

    Orders* pOrd = nullptr;

    int total = 0;
    double totalWait = 0, totalServ = 0;

    // Popping from the Stack naturally prints in descending order of Finish Time (TF)
    while (Finished_Orders.pop(pOrd)) {
        if (pOrd == nullptr) continue; // Safety check

        int Tc = pOrd->getTR() - pOrd->getTA();
        int Tw = 0;
        int Tserv = 0;
        int printed_TS = 0; 

       
        if (pOrd->getType() == TYPE_OT) {
            Tw = pOrd->getTA() - pOrd->getTQ();
            Tserv = pOrd->getTF() - pOrd->getTR();
            printed_TS = 0;
        }
        else {
            
            int TS_Value = 0;

            if (pOrd->getType() == TYPE_ODG || pOrd->getType() == TYPE_ODN) {
                Dineorders* pDine = dynamic_cast<Dineorders*>(pOrd);
                if (pDine) TS_Value = pDine->getTS();
            }
            else {
                
                Deliveryorders* pDelv = dynamic_cast<Deliveryorders*>(pOrd);
                if (pDelv) TS_Value = pDelv->getTS();
            }

            
            Tw = (pOrd->getTA() - pOrd->getTQ()) + (TS_Value - pOrd->getTR());
            Tserv = pOrd->getTF() - TS_Value;
            printed_TS = TS_Value;
        }

        total++;
        totalWait += Tw;
        totalServ += Tserv;

        outFile << pOrd->getTF() << "\t"
            << pOrd->getID() << "\t"
            << pOrd->getTQ() << "\t"
            << pOrd->getTA() << "\t"
            << pOrd->getTR() << "\t"
            << printed_TS << "\t"
            << Tc << "\t"
            << Tw << "\t"
            << Tserv << "\n";
    }

    outFile << "\n------------------------------------------------\n";
    outFile << "------------------ Statistics ------------------\n";
    outFile << "------------------------------------------------\n";
    outFile << "Total Orders: " << total << "\n";
    outFile << "Avg Wait Time: " << (total ? totalWait / total : 0.0) << "\n";
    outFile << "Avg Service Time: " << (total ? totalServ / total : 0.0) << "\n";

    outFile.close();
}

////////////////////////// Logic functions /////////////////////////////////////////////

void Restaurant::AssignPendingToChef(int currentTimestep) {
    Chefs* pChf = nullptr;

    auto assignLogic = [&](Orders* ord, Chefs* chf) {
        ord->setAssignedChef(chf);
        ord->setTA(currentTimestep);
        int cookPeriod = (ord->getSize() + chf->getSpeed() - 1) / chf->getSpeed();
        int readyTime = currentTimestep + cookPeriod;
        ord->setTR(readyTime);
        chf->setFinishTime(readyTime);
        Cooking_Orders.enqueue(ord, -readyTime);
        };

    Dineorders* pDine = nullptr;
    while (!PEND_ODG.isEmpty() && !Free_CS.isEmpty()) {
        PEND_ODG.dequeue(pDine);
        Free_CS.dequeue(pChf);
        assignLogic(pDine, pChf);
    }
    while (!PEND_ODN.isEmpty() && (!Free_CN.isEmpty() || !Free_CS.isEmpty())) {
        PEND_ODN.dequeue(pDine);
        if (!Free_CN.isEmpty()) Free_CN.dequeue(pChf);
        else Free_CS.dequeue(pChf);
        assignLogic(pDine, pChf);
    }

    Takeawayorders* pTake = nullptr;
    while (!PEND_OT.isEmpty() && !Free_CN.isEmpty()) {
        PEND_OT.dequeue(pTake);
        Free_CN.dequeue(pChf);
        assignLogic(pTake, pChf);
    }

    Deliveryorders* pDelv = nullptr;
    int pri;
    while (!PEND_OVG.isEmpty() && !Free_CS.isEmpty()) {
        PEND_OVG.dequeue(pDelv, pri);
        Free_CS.dequeue(pChf);
        assignLogic(pDelv, pChf);
    }
    while (!PEND_OVC.isEmpty() && (!Free_CN.isEmpty() || !Free_CS.isEmpty())) {
        PEND_OVC.dequeue(pDelv);
        if (!Free_CN.isEmpty()) Free_CN.dequeue(pChf);
        else Free_CS.dequeue(pChf);
        assignLogic(pDelv, pChf);
    }
    while (!PEND_OVN.isEmpty() && !Free_CN.isEmpty()) {
        PEND_OVN.dequeue(pDelv);
        Free_CN.dequeue(pChf);
        assignLogic(pDelv, pChf);
    }
}
void Restaurant::finalizeTakeawayOrders(int currentTimestep) {
    Takeawayorders* pTake = nullptr;

    while (READY_OT.peek(pTake)) {
        if (pTake == nullptr) break;

        // Takeaway orders wait exactly 1 timestep after TR to be packed
        if (currentTimestep >= pTake->getTR() + 1) {
            READY_OT.dequeue(pTake);

            
            pTake->setTF(currentTimestep);

            // Finished_Orders (Stack<Orders*>) accepts Takeawayorders* gracefully
            Finished_Orders.push(pTake);
        }
        else {
            break;
        }
    }
}

////////////////////////// Main simulation Function ////////////////////////////////////

void Restaurant::RunSimulator()
{
// reads input file , initialize the restaurant , move to action list
    loadInputFile(); 

 // first print all restaurant parameters
    ui.PrintCurrentState(
        0,
        Request, Cancel,
        PEND_ODG, PEND_ODN, PEND_OT,
        PEND_OVN, PEND_OVC, PEND_OVG,
        Free_CS, Free_CN,
        READY_OD, READY_OT, READY_OV,
        Cooking_Orders, InServ_Orders,
        Finished_Orders, Canceled_Orders,
        Free_Scooters, Back_Scooters,
        Maint_Scooters,
        Free_Tables, Busy_Sharable,
        Busy_NonSharable
    );

// main loop 
    int currentTimestep = 1; 
    while (true)
    {
		Actions* act = nullptr;
        // To Do 0: Execute actions 
        while (!Request.isEmpty()) {
            Request.dequeue(act);
            act->ACT();
            delete act; // Clean up after execution
        }
    /// To Do 1: check Scooters (Back , Maint) ->free

/// To Do 2: check Tables (Busy) -> available 

/// To Do 3: check Finished orders 

/// Assign pending to Chef 
    AssignPendingToChef(currentTimestep);

///To Do 4: move cooking to ready 


/// To Do 6: Assign Ready orders 
    //finalize takeaway orders 
    finalizeTakeawayOrders(currentTimestep);

    //Assign table 
    //Assign Scooter

/// To Do 6: Collect stats 

/// print current stats
    ui.PrintCurrentState(
        currentTimestep,
        Request, Cancel,
        PEND_ODG, PEND_ODN, PEND_OT,
        PEND_OVN, PEND_OVC, PEND_OVG,
        Free_CS, Free_CN,
        READY_OD, READY_OT, READY_OV,
        Cooking_Orders, InServ_Orders,
        Finished_Orders, Canceled_Orders,
        Free_Scooters, Back_Scooters,
        Maint_Scooters,
        Free_Tables, Busy_Sharable,
        Busy_NonSharable
    );
  
/// Check if simulation ends
    int pending = PEND_ODG.getcount() + PEND_ODN.getcount() + PEND_OT.getcount() + PEND_OVN.getcount() + PEND_OVC.getcount() + PEND_OVG.getcount() + Request.getcount() + Cancel.getcount();
    int active = Cooking_Orders.getcount() + READY_OD.getcount() + READY_OT.getcount() + READY_OV.getcount() + InServ_Orders.getcount();
    if (pending == 0 && active == 0) break;
    currentTimestep++;
}
    /// Generate output file 
    generateOutputFile();
    /// print final status
    ui.simulation_ended(currentTimestep, Finished_Orders.getcount(), Canceled_Orders.getcount());



}


