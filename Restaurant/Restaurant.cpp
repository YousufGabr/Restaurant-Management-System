#include "Restaurant.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Restaurant::Restaurant()
{
    srand((unsigned)time(0));

    // Initialize Chefs
    for (int i = 1; i <= 10; i++)
        Free_CS.enqueue(new Chefs(i, Chefs::TYPE_CS, 2));

    for (int i = 11; i <= 35; i++)
        Free_CN.enqueue(new Chefs(i, Chefs::TYPE_CN, 3));

    // Initialize Scooters
    for (int i = 1; i <= 20; i++)
    {
        Scooters* S = new Scooters(i, 150, 23, 8);
        Free_Scooters.enqueue(S, S->getFreePriority());
    }

    // Initialize Tables
    int caps[] = { 3,3,4,4,5,5,5,5,5,6,6,6,7,7,7,7,7,8,8,8 };
    for (int i = 0; i < 20; i++)
    {
        Tables* t = new Tables(i + 1, caps[i]);
        Free_Tables.enqueue(t, t->getPriority());
    }
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

////////////////////////////////////  Load input file to actions ///////////////////////////

void Restaurant::loadInputFile(string filename)
{
    Actions* act = nullptr;
    ifstream input(ui.getinputfilename());
    if (!input.is_open()) {
       cout << "Error file cannot open\n"; return;
    }

    //main data
    int numCN, numCS, speedCN, speedCS;
    int sCount, sSpeed, mainOrds, mainDur;
    int totalTables;
    int tablecount, capacity;  // (3 ,4 ) -> 3 tables with capacity 4;
    int TH;
    int M; // number of actions 
    
  // Actions parameters
    char Acttype = 0;
    string ordtype;
    int TQ;
    int Tcancel;
    int ID;
    int size;
    int price;
    int numberofseats;
    int Duration;
    int distance;
    bool canshare;

 //initalizing the resturant 
    
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
   
    for (int i = 0; i < totalTables;) {
    
    input >> tablecount >> capacity;
    for (int j = 0; j < tablecount; j++) {
      Tables* newTable = new Tables(++i, capacity);
      Free_Tables.enqueue(newTable, capacity);
      }
    }

    input >> TH; // overwhight threshold -> to be implemented


    input >> M; // number of action lines

    //Reading actions and load them to lists
    for(int i =0 ; i<M; i++)
    {
        input >> Acttype;
        if (Acttype == 'Q')
        {
            

            input >> ordtype;

            if (ordtype == "ODG")
            {
                input >> TQ >> ID >> size >> price >> numberofseats >> Duration >> canshare;
                act = new RequestAction(this, ID, TYPE_ODG, TQ, size, price,
                    numberofseats, Duration, canshare);
            }

            else if (ordtype == "ODN")
            {
                input >> TQ >> ID >> size >> price >> numberofseats >> Duration >> canshare;
                act = new RequestAction(this, ID, TYPE_ODN, TQ, size, price,
                    numberofseats, Duration, canshare);
            }
            else if (ordtype == "OT")
            {
                input >> TQ >> ID >> size >> price ;
                act = new RequestAction(this, ID, TYPE_OT, TQ, size, price);
            }
            else if (ordtype == "OVN")
            {
                input >> TQ >> ID >> size >> price >> distance;
                act = new RequestAction(this, ID, TYPE_OVN, TQ, size, price, distance);
            }
            else if (ordtype == "OVG")
            {
                input >> TQ >> ID >> size >> price >> distance;
                act = new RequestAction(this, ID, TYPE_OVG, TQ, size, price, distance);
            }
            else if (ordtype == "OVN")
            {
                input >> TQ >> ID >> size >> price >> distance;
                act = new RequestAction(this, ID, TYPE_OVC, TQ, size, price, distance);
            }

            if(act)
            Request.enqueue(act);

        }
        else if (Acttype == 'X')
        {
            input >> Tcancel>>ID;
            act = new CancelAction(this, Tcancel, ID);

            if(act)
            Cancel.enqueue(act);

        }
    }

    input.close(); 

}

////////////////////////// Main simulation Function ////////////////////////////////////

/*void Restaurant::RunSimulator()
{
  
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

    int timestep = 1;

    while (true)
    {
        /// 3.1: Move Pending to Cooking
        for (int i = 0; i < 30; i++)
        {
            if (Free_CS.isEmpty() && Free_CN.isEmpty()) break;
            if (PEND_ODG.isEmpty() && PEND_ODN.isEmpty() && PEND_OT.isEmpty() &&
                PEND_OVN.isEmpty() && PEND_OVC.isEmpty() && PEND_OVG.isEmpty()) break;

            Orders* ord = nullptr;
            int p = 0;
            bool dequeued = false;

            while (!dequeued) {
                int choice = rand() % 6; // Generate a number 0-5

                switch (choice) {
                case 0:
                    if (!PEND_ODG.isEmpty()) { PEND_ODG.dequeue(ord); dequeued = true; }
                    break;
                case 1:
                    if (!PEND_ODN.isEmpty()) { PEND_ODN.dequeue(ord); dequeued = true; }
                    break;
                case 2:
                    if (!PEND_OT.isEmpty()) { PEND_OT.dequeue(ord); dequeued = true; }
                    break;
                case 3:
                    if (!PEND_OVN.isEmpty()) { PEND_OVN.dequeue(ord); dequeued = true; }
                    break;
                case 4:
                    if (!PEND_OVC.isEmpty()) { PEND_OVC.dequeue(ord); dequeued = true; }
                    break;
                case 5:
                    if (!PEND_OVG.isEmpty()) { PEND_OVG.dequeue(ord, p); dequeued = true; }
                    break;
                }
            }

            if (!ord) continue;

            Chefs* chf = nullptr;
            if (!Free_CS.isEmpty() && (Free_CN.isEmpty() || rand() % 2 == 0)) Free_CS.dequeue(chf);
            else Free_CN.dequeue(chf);

            ord->setTA(timestep);
            ord->setAssignedChef(chf);
            Cooking_Orders.enqueue(ord, (int)(ord->getPriority() * 1000));
        }

        /// 3.2: Move Cooking to Ready
        for (int i = 0; i < 15; i++)
        {
            if (Cooking_Orders.isEmpty()) break;
            if ((rand() % 100) < 75)
            {
				int pri = 0;
                Orders* ord = nullptr; Cooking_Orders.dequeue(ord, pri);
                if (ord) {
                    Chefs* chf = ord->getAssignedChef();
                    if (chf) {
                        if (chf->getType() == Chefs::TYPE_CS) Free_CS.enqueue(chf);
                        else Free_CN.enqueue(chf);
                    }
                    ord->setTR(timestep);
					ord->setAssignedChef(nullptr);
                    if (ord->getType() == TYPE_ODG || ord->getType() == TYPE_ODN) READY_OD.enqueue(ord);
                    else if (ord->getType() == TYPE_OT) READY_OT.enqueue(ord);
                    else READY_OV.enqueue(ord);
                }
            }
        }

        /// 3.3: Move Ready to In-Service 
        for (int i = 0; i < 10; i++)
        {
            int choice = rand() % 3;
            Orders* ord = nullptr; int p = 0;

            if (!READY_OT.isEmpty() && choice == 0) {
                READY_OT.dequeue(ord);
                ord->setTS(timestep); ord->setTF(timestep + 1);
                Finished_Orders.push(ord);
            }
            else if (!READY_OD.isEmpty() && choice == 1) {
                Orders* ord = nullptr;
                READY_OD.peek(ord);
                Tables* tbl = nullptr;

                // 1. Opt for table sharing first
                if (ord->isSharable() && !Busy_Sharable.isEmpty()) {
                    tbl = Busy_Sharable.getBest(ord);
                }

                // 2. If no suitable busy table was found, check Free_Tables
                if (tbl == nullptr && !Free_Tables.isEmpty()) {
                    tbl = Free_Tables.getBest(ord);
                }

                // 3. If a table was successfully found in EITHER list
                if (tbl != nullptr) {
                    READY_OD.dequeue(ord); // Safe to remove from ready list now
                    ord->setAssignedTable(tbl);

                    if (ord->isSharable()) {
                        tbl->set_free_Seats(tbl->get_free_Seats() - ord->getNoOfSeats());
                        if(tbl->get_free_Seats() == 0) Busy_NonSharable.enqueue(tbl, tbl->getPriority());
                        else Busy_Sharable.enqueue(tbl, tbl->getPriority());
                    }
                    else {
                        tbl->set_free_Seats(tbl->get_free_Seats() - ord->getNoOfSeats());
                        Busy_NonSharable.enqueue(tbl, tbl->getPriority());
                    }

                    ord->setTS(timestep);
                    InServ_Orders.enqueue(ord, (int)(ord->getPriority() * 1000));
                }
                
            }
            else if (!READY_OV.isEmpty() && !Free_Scooters.isEmpty() && choice == 2) {


                READY_OV.dequeue(ord);
                Scooters* sc = nullptr; Free_Scooters.dequeue(sc, p);
                if (sc) {
                    sc->setTotalDistance(ord->getDistance());
                    sc->assignOrder(ord, timestep);
                    ord->setAssignedScooter(sc);
                    ord->setTS(timestep); InServ_Orders.enqueue(ord, (int)(ord->getPriority() * 1000));
                }
            }
        }

		///3.4 & 3.5 & 3.6: Cancel OVC Orders Randomly
		CancelOVC(rand() % 500 + 1); 

        /// 3.7: Service Finish
        if ((rand() % 100) < 25 && !InServ_Orders.isEmpty())
        {
			int p = 0;
            Orders* ord = nullptr; InServ_Orders.dequeue(ord,p);
            if (ord) {
                ord->setTF(timestep);
                Finished_Orders.push(ord);
                if (ord->getType() >= 3) { 
                    Scooters* sc = ord->getAssignedScooter();
                    if (sc) {
                        
                        Back_Scooters.enqueue(sc, sc->getPriority());
                        ord->setAssignedScooter(nullptr);
                    }
                }
                else if (ord->getType() < 2) { // Dine-in
                    Tables* tbl = nullptr;
                    PriorityQueue<Tables*> tempnoshare, tempshare; int pri = 0;
                    bool found = false;
                    while (Busy_NonSharable.dequeue(tbl, pri)) {
                        if (!found && tbl==ord->getAssignedTable()) 
                        {
                            tbl->set_free_Seats(tbl->get_free_Seats() + ord->getNoOfSeats());
                            if (tbl->is_free())
                            {
                                Free_Tables.enqueue(tbl, tbl->getPriority());
                            }
                            else if (tbl->get_free_Seats() > 0) Busy_Sharable.enqueue(tbl, tbl->getPriority());
                            found = true; 
                        }
                        else tempnoshare.enqueue(tbl, tbl->getPriority());
                    }
                    while (tempnoshare.dequeue(tbl, pri)) Busy_NonSharable.enqueue(tbl, pri);

                    while (Busy_Sharable.dequeue(tbl, pri)) {
                        if (!found && tbl == ord->getAssignedTable()) { 
							tbl->set_free_Seats(tbl->get_free_Seats() + ord->getNoOfSeats()); // Update free seats for sharable tables
                            if (tbl->is_free())
                            {
                                Free_Tables.enqueue(tbl, tbl->getPriority());
                            }
                            else tempshare.enqueue(tbl, tbl->getPriority());
                            found = true; 
                        }
                        else tempshare.enqueue(tbl, pri);
                    }
                    while (tempshare.dequeue(tbl, pri)) Busy_Sharable.enqueue(tbl, pri);
                    ord->setAssignedTable(nullptr);
                }
            }
        }

        /// 3.8: Returning Scooters (Back -> Maint/Free)
        if (!Back_Scooters.isEmpty() && (rand() % 100 < 50)) {
            int p = 0;
            Scooters* sc = nullptr; Back_Scooters.dequeue(sc, p);
            if (rand() % 2 == 0) Free_Scooters.enqueue(sc, p);
            else Maint_Scooters.enqueue(sc);
        }

        /// 3.9: Maintenance (Maint -> Free)
        if (!Maint_Scooters.isEmpty() && (rand() % 100 < 50)) {
            Scooters* sc = nullptr; Maint_Scooters.dequeue(sc);
            Free_Scooters.enqueue(sc, sc->getPriority());
        }

		///3.10: Print Current State
        ui.PrintCurrentState(
            timestep,
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

		///4: Check for termination condition (no pending or active orders)
        int pending = PEND_ODG.getcount() + PEND_ODN.getcount() + PEND_OT.getcount() + PEND_OVN.getcount() + PEND_OVC.getcount() + PEND_OVG.getcount();
        int active = Cooking_Orders.getcount() + READY_OD.getcount() + READY_OT.getcount() + READY_OV.getcount() + InServ_Orders.getcount();
        if (pending == 0 && active == 0) break;
        timestep++;

    }
	ui.simulation_ended(timestep, Finished_Orders.getcount(), Canceled_Orders.getcount());
} */





void Restaurant::generateOutputFile(string filename) {
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
        int printed_TS = 0; // «·ﬁÌ„… «· Ì ” ıÿ»⁄ ›Ì «·„·›

        // «· Õﬁﬁ „‰ ‰Ê⁄ «·ÿ·» ·„⁄«·Ã… €Ì«» «·‹ TS ›Ì «· Ìﬂ √Ê«Ì
        if (pOrd->getType() == TYPE_OT) {
            Tw = pOrd->getTA() - pOrd->getTQ();
            Tserv = pOrd->getTF() - pOrd->getTR();
            printed_TS = 0;
        }
        else {
            // »„« √‰ «·ﬂ·«” «·√» Orders ·« Ì„ ·ﬂ TS° ‰ﬁÊ„ »⁄„· Cast ··ﬂ·«” «·›—⁄Ì
            int TS_Value = 0;

            if (pOrd->getType() == TYPE_ODG || pOrd->getType() == TYPE_ODN) {
                Dineorders* pDine = dynamic_cast<Dineorders*>(pOrd);
                if (pDine) TS_Value = pDine->getTS();
            }
            else {
                // ··ÿ·»«  «·œ·Ì›—Ì OVG, OVC, OVN
                Deliveryorders* pDelv = dynamic_cast<Deliveryorders*>(pOrd);
                if (pDelv) TS_Value = pDelv->getTS();
            }

            // «·¬‰ ‰” Œœ„ TS_Value «·„Õ”Ê»… ›Ì «·„⁄«œ·« 
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

// Logic to be placed in the Restaurant class
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

            // ·« ÌÊÃœ TS Â‰«° ‰‰ ﬁ· ··‹ TF (Êﬁ  «·‰Â«Ì…) „»«‘—…
            pTake->setTF(currentTimestep);

            // Finished_Orders (Stack<Orders*>) accepts Takeawayorders* gracefully
            Finished_Orders.push(pTake);
        }
        else {
            break;
        }
    }
}