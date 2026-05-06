#include "Restaurant.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Restaurant::Restaurant() : Mode(-1)
{
    
}



////////////////////////  ADD and Cancel Functions //////////////////////

void Restaurant::AddPendingOrder(Orders* neworder , int currentTimestep)
{
    ORD_TYPE orderType = neworder->getType();
    if (dynamic_cast<ComboOrders*>(neworder))
    {
        ComboOrders* newCM = dynamic_cast<ComboOrders*>(neworder);
        if (newCM)
            PENDING_COMBO.enqueue(newCM, newCM->getpendingPriority());
        newCM->setTQ(currentTimestep);
    }
    else if (dynamic_cast<Deliveryorders*>(neworder))
    {
        Deliveryorders* newOV = dynamic_cast<Deliveryorders*>(neworder);
        switch (orderType)
        {
        case TYPE_OVN: PEND_OVN.enqueue(newOV); break;
        case TYPE_OVC: PEND_OVC.enqueue(newOV); break;
        case TYPE_OVG: PEND_OVG.enqueue(newOV, (newOV->getOVGpriority())); break;
        }
        newOV->setTQ(currentTimestep);

    }
    else if (dynamic_cast<Dineorders*>(neworder))
    {
        Dineorders* newOD = dynamic_cast<Dineorders*>(neworder);
        switch (orderType)
        {
        case TYPE_ODG: PEND_ODG.enqueue(newOD); break;
        case TYPE_ODN: PEND_ODN.enqueue(newOD); break;
        }
        newOD->setTQ(currentTimestep);
    }
    else if (dynamic_cast<Takeawayorders*>(neworder))
    {
        Takeawayorders* newOT = dynamic_cast<Takeawayorders*>(neworder);
        PEND_OT.enqueue(newOT);
        newOT->setTQ(currentTimestep);
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
    string filename = ui.getinputfilename();
    ifstream input(filename);

    while (!input.is_open()) {
        cout << "Error file cannot open\n";
        cout << RED << "----------------------" << RESET << endl;
        cout << RED << "Error file cannot open" << RESET << endl;
        cout << RED << "----------------------" << RESET << endl;
        filename = ui.getinputfilename();
        input.open(filename);
    }

    // 1. Declare all variables ONCE at the top
    int numCN, numCS, speedCN, speedCS;
    int snCount, srCount, sSpeed, mainOrds, mainDur;
    int totalTables;
    int tablecount, capacity;
    int TH;
    int M;
    //data for combo orders
    int chefsNumber; int CN; int CS; int ScootersNumber;

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

    
    input >> snCount >> srCount >> sSpeed >> mainOrds >> mainDur;
    for (int i = 0; i < snCount; i++) {
        Scooters* s = new Scooters(SC_TYPE::TYPE_NORMAL, i + 1, sSpeed, mainDur, mainOrds);
        Free_Scooters.enqueue(s, s->getFreePriority());
    }
    for (int i = 0; i < srCount; i++) {
        Scooters* s = new Scooters(SC_TYPE::TYPE_RESCUE, i + 1, sSpeed, mainDur, mainOrds);
        Resc_Scooters.enqueue(s);
    }

    
    input >> totalTables;
    for (int i = 0; i < totalTables; /* incrementing handled inside */) {
        input >> tablecount >> capacity;
        for (int j = 0; j < tablecount; j++) {
            Tables* newTable = new Tables(++i, capacity);
            Free_Tables.enqueue(newTable, newTable->getPriority());
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

            if (ordtype == "COMBO")
            {
                input >> TQ >> ID >> size >> price >> distance>>chefsNumber >> CN >> CS>> ScootersNumber;
                act = new RequestAction(this, ID, TYPE_COMBO, TQ, size, price, distance, chefsNumber, CN, CS, ScootersNumber);

            }

            else if (ordtype == "ODG") {
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
                act = new RequestAction(this, ID, TYPE_OVG, TQ, size, price, distance, TH);
            }
            else if (ordtype == "OVC") { 
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

void Restaurant::generateOutputFile(int currentTimestep)
{
    string filename = ui.getoutputfilename();
    ofstream outFile(filename);

    Orders* pOrd = nullptr;

    int total_finished = 0;
    int total_overwait = 0;
    int total_ovg = 0;
    int finished_OD = 0, finished_OT = 0, finished_OV = 0, finished_COMBO = 0;

    double sum_Ti = 0, sum_Tc = 0, sum_Tw = 0, sum_Tserv = 0;
    double total_chef_busy_time = 0;
    double total_scooter_busy_time = 0;

    ArrayStack<Orders*> tempStack = Finished_Orders;
    outFile << "================== ORDERS REPORT ==================\n";
    outFile << "TF\tID\tTQ\tTA\tTR\tTS\tTi\tTc\tTw\tTserv\n";

    while (tempStack.pop(pOrd))
    {
        int TF = pOrd->getTF();
        int TQ = pOrd->getTQ();
        int TA = pOrd->getTA();
        int TR = pOrd->getTR();
        int Tc = pOrd->getTC();
        int Ti = 0;
        int Tw = 0;
        if (dynamic_cast<Takeawayorders*>(pOrd))
        {
            Takeawayorders* Pt = dynamic_cast<Takeawayorders*>(pOrd);
            Ti = Pt->getTi();
            Tw = Pt->getTW();
        }
        else
        {
            Ti = pOrd->getTi();
            Tw = pOrd->getTW();
        }
       

        int TS = 0;
        int Tserv = 0;

        ORD_TYPE type = pOrd->getType();

        if (type == TYPE_OT)
        {
            finished_OT++;
            total_chef_busy_time += Tc;
            Tserv = 1;
            TS = TR; // service begins once Ready (Packing)
           
        }
        else if (type == TYPE_COMBO)
        {
            finished_COMBO++;
            ComboOrders* cmb = dynamic_cast<ComboOrders*> (pOrd);

            TS = cmb->getTs();
            Tserv = cmb->getTserv();

            total_chef_busy_time += Tc * cmb->getChefsNumber();
            total_scooter_busy_time += Tserv * cmb->getScootersNumber();
        }
        else if (type == TYPE_ODG || type == TYPE_ODN)
        {
            finished_OD++;
            Dineorders* dine = dynamic_cast<Dineorders*> (pOrd);

            TS = dine->getTS();
            Tserv = dine->getorderDuration();

            total_chef_busy_time += Tc;
        }
        else
        {
            finished_OV++;
            Deliveryorders* delv = dynamic_cast<Deliveryorders *> (pOrd);
            
            if (delv->getow()) total_overwait++;
            if (pOrd->getType() == TYPE_OVG) total_ovg++;
             TS = delv->getTS();
             Tserv = delv->getTserv();

             total_chef_busy_time += Tc;
             total_scooter_busy_time += 2*Tserv;
           
            
        }

        sum_Ti += Ti;
        sum_Tc += Tc;
        sum_Tw += Tw;
        sum_Tserv += Tserv;
        total_finished++;
        

        outFile << TF << "\t" << pOrd->getID() << "\t" << TQ << 
            "\t" << TA << "\t" << TR << "\t" << TS << 
            "\t" << Ti << "\t" << Tc << "\t" << Tw << "\t" << Tserv << "\n";

    }

    int total_CN = Free_CN.getcount();
    int total_CS = Free_CS.getcount();
    int total_chefs = total_CN + total_CS;

    int total_scooters = Free_Scooters.getcount() + Resc_Scooters.getcount();

    int total_cancelled = Canceled_Orders.getcount();
    

    int total_orders = total_finished + total_cancelled;

    int cancelled_OD = 0, cancelled_OT = 0, cancelled_OV = 0, cancelled_COMBO = 0;
    ArrayStack<Orders*> tempStack2;
    Orders* pOrd2;

    while (!Canceled_Orders.isEmpty()) {
        Canceled_Orders.pop(pOrd2);

        if (pOrd2 != nullptr) {
            int type = pOrd2->getType();
            if (type == TYPE_ODN) cancelled_OD++;
            if (type == TYPE_ODG) cancelled_OD++;
            else if (type == TYPE_OT) cancelled_OT++;
            else if (type == TYPE_OVN) cancelled_OV++;
            else if (type == TYPE_OVG) cancelled_OV++;
            else if (type == TYPE_OVC) cancelled_OV++;
            else if (type == TYPE_COMBO) cancelled_COMBO++;

            tempStack2.push(pOrd2); // Keep it to restore later
        }
    }

    
    while (!tempStack2.isEmpty()) {
        tempStack2.pop(pOrd2);
        Canceled_Orders.push(pOrd2);
    }

    int total_OD = finished_OD + cancelled_OD;
    int total_OT = finished_OT + cancelled_OT;
    int total_OV = finished_OV + cancelled_OV;
    int total_COMBO = finished_COMBO + cancelled_COMBO;

    outFile << "\n=====================================================\n";
    outFile << "           RESTAURANT SIMULATION REPORT \n";
    outFile << "=====================================================\n\n";

    outFile << "------------------ ORDER SUMMARY ------------------\n";
    outFile << "Total Orders     : " << total_orders << "\n";
    outFile << "   -> Dine-in (OD): " << total_OD << "\n";
    outFile << "   -> Takeaway   : " << total_OT << "\n";
    outFile << "   -> Delivery   : " << total_OV << "\n";
    outFile << "   -> Combo      : " << total_COMBO << "\n\n";

    outFile << "------------------ RESOURCE SUMMARY ------------------\n";
    outFile << "Total Chefs      : " << total_chefs << "\n";
    outFile << "    -> Normal (CN): " << total_CN << "\n";
    outFile << "    -> Special(CS): " << total_CS << "\n";
    outFile << "Total Scooters   : " << total_scooters << "\n";
    outFile << "    -> Normal (S): " << Free_Scooters.getcount() << "\n";
    outFile << "    -> Special(SR): " << Resc_Scooters.getcount() << "\n\n";

    outFile << "------------------ PERFORMANCE STATS ------------------\n";
    outFile << " Finished Orders : "
        << ((total_finished * 1.0) / total_orders) * 100 << "%\n";
    outFile << " Cancelled Orders: "
        << ((total_cancelled * 1.0) / total_orders) * 100 << "%\n";
    outFile << " Overwait Orders : "
        << ((total_overwait * 1.0) / total_ovg) * 100 << "%\n\n";

    outFile << "------------------ TIME ANALYSIS ------------------\n";
    outFile << "Avg Idle Time (Ti)    : " << (sum_Ti * 1.0) / total_finished << "\n";
    outFile << "Avg Cooking Time (Tc)  : " << (sum_Tc * 1.0) / total_finished << "\n";
    outFile << "Avg Wait Time (Tw)     : " << (sum_Tw * 1.0) / total_finished << "\n";
    outFile << "Avg Service Time       : " << (sum_Tserv * 1.0) / total_finished << "\n\n";

    outFile << "------------------ UTILIZATION ------------------\n";
    outFile << " Scooters Utilization: "
        << ((total_scooter_busy_time * 1.0) /
            (currentTimestep * total_scooters)) * 100
        << "%\n";

    outFile << " Chefs Utilization   : "
        << ((total_chef_busy_time * 1.0) /
            (currentTimestep * total_chefs)) * 100
        << "%\n\n";

    outFile << "=====================================================\n";
    outFile << "               END OF SIMULATION REPORT\n";
    outFile << "=====================================================\n";

    outFile.close();;
}

////////////////////////// Logic functions /////////////////////////////////////////////


void Restaurant::executeActions(int currenttimestep)
{
    Actions* a = nullptr;

    
    while (true)
    {
        a = nullptr;
        Request.peek(a);
        if (!a) break;
        if (a->getTimestep() <= currenttimestep)
        {
            Request.dequeue(a);
            a->ACT();
        }
        else break;
    }

    
    while (true)
    {
        a = nullptr;
        Cancel.peek(a);
        if (!a) break;
        if (a->getTimestep() <= currenttimestep)
        {
            Cancel.dequeue(a);
            a->ACT();
        }
        else break;
    }
}

void Restaurant::checkScootersAvailablity(int currentTimestep)
{
    Scooters* s = nullptr;
    while(!Maint_Scooters.isEmpty())
    {
        Maint_Scooters.peek(s);
        if (s)
        {
            if (currentTimestep - 2* s->getfinish_time() + s->get_StartTime() >= s->get_Maintenance_Duration())
            {
                Maint_Scooters.dequeue(s);
                if (s->getType() == SC_TYPE::TYPE_RESCUE) Resc_Scooters.enqueue(s);
                else Free_Scooters.enqueue(s, s->getFreePriority());
            }
            else break;
        }
        else break;
    }
    

    int pri = 0;
    while (!Back_Scooters.isEmpty())
    {
        Back_Scooters.peek(s, pri);
        if (s)
        {
            if (currentTimestep - s->getfinish_time() >= (s->getfinish_time() - s->get_StartTime()))
            {
                Back_Scooters.dequeue(s, pri);
                if (s->getCount() >= s->get_MaxTripsBeforaMaint() || s->get_Breakdown())
                {
                    // fix updates scooters abillity to to more Maint_Ords  orders ;
                    s->fix();
                    s->set_Breakdown(false);
                    Maint_Scooters.enqueue(s);
                }
                else
                {
                    if(s->getType() == SC_TYPE::TYPE_RESCUE) Resc_Scooters.enqueue(s);
					else Free_Scooters.enqueue(s, s->getFreePriority());
                }
            }
            else break;
        }
        else break;

    }
   
}


void Restaurant::checkFinishedOrders(int currentTimestep)
{
    Orders* ord = nullptr;
    int pri = 0;
    while (InServ_Orders.peek(ord, pri))
    {
        if (ord == nullptr) break;
        if (dynamic_cast<ComboOrders*>(ord))
        {
            ComboOrders* cmb = dynamic_cast<ComboOrders*>(ord);
            if (currentTimestep - cmb->getTs() >= cmb->getDeliveryDuration())
            {
                InServ_Orders.dequeue(ord, pri);
                if (ord)
                {
                    Scooters** s = cmb->getAssignedScooters();
                    for (int i = 0; i < cmb->getScootersNumber(); i++)
                    {
                        s[i]->setfinish_time(currentTimestep);
                        s[i]->updateCount();
                        Back_Scooters.enqueue(s[i], s[i]->getBackPriority());

                    }

                    cmb->setAssignedScooters(nullptr);
                    delete[] s;
                    s = nullptr;
                    Finished_Orders.push(ord);
                    ord->setTF(currentTimestep);
                }

            }
            else break;

        }
        else if (dynamic_cast<Dineorders*>(ord))
        {
            Dineorders* dine = dynamic_cast<Dineorders*>(ord);
            if ((currentTimestep - dine->getTS()) >= dine->getorderDuration())
            {
                InServ_Orders.dequeue(ord, pri);
				Finished_Orders.push(ord);
                dine->setTF(currentTimestep);
                Tables* tbl = nullptr;
                PriorityQueue<Tables*> tempnoshare, tempshare;
                bool found = false;
                while (Busy_NonSharable.dequeue(tbl, pri)) {
                    if (!found && tbl == dine->getAssignedTable())
                    {
                        tbl->set_free_Seats(tbl->get_free_Seats() + dine->getNoOfSeats());
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
                    if (!found && tbl == dine->getAssignedTable()) {
                        tbl->set_free_Seats(tbl->get_free_Seats() + dine->getNoOfSeats()); // Update free seats for sharable tables
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
                dine->setAssignedTable(nullptr);
            }
            else break;
        }
        else if (dynamic_cast<Deliveryorders*>(ord))
        {
            Deliveryorders* delv = dynamic_cast<Deliveryorders*>(ord);
            if ((currentTimestep - delv->getTS()) >= delv->getDeliveryDuration())
            {
                InServ_Orders.dequeue(ord, pri);
                if (ord)
                {
                    Scooters* s = delv->getAssignedScooter();
                    s->setfinish_time(currentTimestep);
                    s->updateCount();
                    delv->setAssignedScooter(nullptr);
                    Finished_Orders.push(ord);
					ord->setTF(currentTimestep);
                    Back_Scooters.enqueue(s, s->getBackPriority());
       
                }
            }
            else break;
        }
		else break;
    }
}



void Restaurant::AssignPendingToChef(int currentTimestep) {
    Chefs* pChf = nullptr;

    Orders* ord = nullptr;

    if (!assignComboTochef(currentTimestep) && !PENDING_COMBO.isEmpty()) return;

    while (!PEND_ODG.isEmpty() && !Free_CS.isEmpty()) {
        PEND_ODG.dequeue(ord);
        Free_CS.dequeue(pChf);
        ord->setAssignedChef(pChf);
        ord->setTA(currentTimestep);
        Cooking_Orders.enqueue(ord, ord->getCookingpriority());
    }
    while (!PEND_ODN.isEmpty() && (!Free_CN.isEmpty() || !Free_CS.isEmpty())) {
        PEND_ODN.dequeue(ord);
        if (!Free_CN.isEmpty()) Free_CN.dequeue(pChf);
        else Free_CS.dequeue(pChf);
        ord->setAssignedChef(pChf);
        ord->setTA(currentTimestep);
        Cooking_Orders.enqueue(ord, ord->getCookingpriority());
        
    }

   
    while (!PEND_OT.isEmpty() && !Free_CN.isEmpty()) {
        PEND_OT.dequeue(ord);
        Free_CN.dequeue(pChf);
        ord->setAssignedChef(pChf);
        ord->setTA(currentTimestep);
        Cooking_Orders.enqueue(ord, ord->getCookingpriority());
    }

    int pri;
    while (!PEND_OVG.isEmpty() && !Free_CS.isEmpty()) {
        PEND_OVG.dequeue(ord, pri);
        Free_CS.dequeue(pChf);
        ord->setAssignedChef(pChf);
        ord->setTA(currentTimestep);
        Cooking_Orders.enqueue(ord, ord->getCookingpriority());
    }
    while (!PEND_OVC.isEmpty() && (!Free_CN.isEmpty() || !Free_CS.isEmpty())) {
        PEND_OVC.dequeue(ord);
        if (!Free_CN.isEmpty()) Free_CN.dequeue(pChf);
        else Free_CS.dequeue(pChf);
        ord->setAssignedChef(pChf);
        ord->setTA(currentTimestep);
        Cooking_Orders.enqueue(ord, ord->getCookingpriority());
        
    }
    while (!PEND_OVN.isEmpty() && !Free_CN.isEmpty()) {
        PEND_OVN.dequeue(ord);
        Free_CN.dequeue(pChf);
        ord->setAssignedChef(pChf);
        ord->setTA(currentTimestep);
        Cooking_Orders.enqueue(ord, ord->getCookingpriority());
    }
}

void Restaurant::MoveToReady(int currentTimestep) {
    Orders* pOrd = nullptr;
    Chefs* pChf = nullptr;
    int pri = 0;
    while (Cooking_Orders.peek(pOrd, pri)) {
        if (pOrd == nullptr) break;
        //handling comobs
        if (pOrd->getType() == TYPE_COMBO)
        {
            
            bool check =moveComboToready(currentTimestep, pOrd);
            if (check == false) break;
            else
            {
                Cooking_Orders.dequeue(pOrd, pri);
                READY_COMBO.enqueue(pOrd);
                pOrd->setTR(currentTimestep);

            }
        }
        else if (currentTimestep - pOrd->getTA() >= pOrd->getCookingOrderDuration()) {
            Cooking_Orders.dequeue(pOrd, pri);
			pOrd->setTR(currentTimestep); // Update TR to the actual time it finished cooking
            pChf = pOrd->getAssignedChef();
            if (pChf) {
                if (pChf->getType() == Chefs::TYPE_CS) Free_CS.enqueue(pChf);
                else Free_CN.enqueue(pChf);
                pOrd->setAssignedChef(nullptr);
            }
            switch (pOrd->getType()) {
                case TYPE_ODG:
                case TYPE_ODN:
                    READY_OD.enqueue(pOrd);
                    break;
                case TYPE_OT:
                    READY_OT.enqueue(pOrd);
                    break;
                case TYPE_OVN:
                case TYPE_OVC:
                case TYPE_OVG:
                    READY_OV.enqueue(pOrd);
                    break;
            }
        }
        else break;
    }
}
void Restaurant::checkOverwaitOVG(int currentTimestep) {

    Orders* pOV = nullptr;
    LinkedQueue<Orders*> tempOVG;
    while (READY_OV.dequeue(pOV)) 
    {
        if (pOV->getType() != TYPE_OVG) tempOVG.enqueue(pOV);

        else
        {
            Deliveryorders* delv = dynamic_cast<Deliveryorders*>(pOV);
            int waitTime = currentTimestep - delv->getTR();
            if (waitTime >= delv->getTH())
            {
                delv->setow(true);
                Overwait_OVG.enqueue(pOV, waitTime);
            }
            else
            {
                tempOVG.enqueue(pOV);
            }
        }
    }
	while (tempOVG.dequeue(pOV)) READY_OV.enqueue(pOV);
}
void Restaurant::finalizeTakeawayOrders(int currentTimestep) {
    Orders* pTake = nullptr;

    while (READY_OT.peek(pTake)) {
        if (pTake == nullptr) break;

        
        if (currentTimestep >= pTake->getTR() + 1) {
            READY_OT.dequeue(pTake);


            pTake->setTF(currentTimestep);

            
            Finished_Orders.push(pTake);
        }
        else break;
    }
}

void Restaurant::MovetoInservice(int currentTimestep) {

    //OD First
    while (!READY_OD.isEmpty()) {
        Orders* ord = nullptr;
        READY_OD.peek(ord);
        Dineorders* dine = dynamic_cast<Dineorders*>(ord);
        Tables* tbl = nullptr;

        
        if (dine->isSharable() && !Busy_Sharable.isEmpty()) {
            tbl = Busy_Sharable.getBest(dine);
        }

       
        if (tbl == nullptr && !Free_Tables.isEmpty()) {
            tbl = Free_Tables.getBest(dine);
        }

       
        if (tbl != nullptr) {
            READY_OD.dequeue(ord); 
            dine->setAssignedTable(tbl);

            if (dine->isSharable()) {
                tbl->set_free_Seats(tbl->get_free_Seats() - dine->getNoOfSeats());
                if (tbl->get_free_Seats() == 0) Busy_NonSharable.enqueue(tbl, tbl->getPriority());
                else Busy_Sharable.enqueue(tbl, tbl->getPriority());
            }
            else {
                tbl->set_free_Seats(tbl->get_free_Seats() - dine->getNoOfSeats());
                Busy_NonSharable.enqueue(tbl, tbl->getPriority());
            }

            dine->setTS(currentTimestep);
            InServ_Orders.enqueue(ord, dine->getServicePriority());
        }
        else break;
    }

    //handling combo orders
    while (!READY_COMBO.isEmpty() && !Free_Scooters.isEmpty())
    {
        Orders* ord = nullptr;
        int pri = 0;
        ComboOrders* cmb = nullptr;
        Scooters*s = nullptr;
        READY_COMBO.peek(ord);
        if (ord) cmb = dynamic_cast<ComboOrders*>(ord);
        if (Free_Scooters.getcount() >= cmb->getScootersNumber())
        {
            READY_COMBO.dequeue(ord);
            Scooters** AssignedScooters = new Scooters * [cmb->getScootersNumber()];
            for (int i = 0; i < cmb->getScootersNumber(); i++)
            {
                Free_Scooters.dequeue(s, pri);
                if (s)
                {
                    AssignedScooters[i] = s;
                    s->updateTotalDistance(cmb->getDistance());
                    s->setTripdistance(cmb->getDistance());
                    s->setstart_time(currentTimestep);
                }

            }
            cmb->setAVGscooters(AssignedScooters[0]->get_Speed());
            cmb->setAssignedScooters(AssignedScooters);
            cmb->setTS(currentTimestep);
            InServ_Orders.enqueue(ord, cmb->getServicePriority());
            
        }
        else return;
        
    }

    //Overwait OVG 
    while (!Overwait_OVG.isEmpty() && !Free_Scooters.isEmpty()) {
        Orders* ord = nullptr;
        int pri = 0;
        Deliveryorders* delv = nullptr;
        Scooters* sc = nullptr;
        Overwait_OVG.dequeue(ord, pri);
        if (ord) delv = dynamic_cast<Deliveryorders*>(ord);
        Free_Scooters.dequeue(sc, pri);

        if (sc && delv) {

            sc->updateTotalDistance(delv->getDistance());
            sc->setTripdistance(delv->getDistance());
            sc->setstart_time(currentTimestep);
            delv->setAssignedScooter(sc);
            delv->setTS(currentTimestep);
            InServ_Orders.enqueue(ord, delv->getServicePriority());
        }
    }
    
    

     //Rest Of OV 
     while (!READY_OV.isEmpty() && !Free_Scooters.isEmpty()) {
         Orders* ord = nullptr;
         int pri = 0;
         Deliveryorders* delv = nullptr;
         Scooters* sc = nullptr;
         READY_OV.dequeue(ord);
         if (ord) delv = dynamic_cast<Deliveryorders*>(ord);
         Free_Scooters.dequeue(sc, pri);

         if (sc && delv) {
             
             sc->updateTotalDistance(delv->getDistance());
             sc->setTripdistance(delv->getDistance());
			 sc->setstart_time(currentTimestep);
             delv->setAssignedScooter(sc);
             delv->setTS(currentTimestep);
             InServ_Orders.enqueue(ord, delv->getServicePriority());
         }
     }
}


// combo related functions //
bool Restaurant::assignComboTochef(int currentTimestep)
{
    Orders* ord = nullptr;
    int pri = 0;
    int Cnspeed = 0;
    int Csspeed = 0;
    bool assigned = false;
   
    while (PENDING_COMBO.peek(ord, pri) && ord->getTQ() <= currentTimestep)
    {

        ComboOrders* cmb = dynamic_cast<ComboOrders*> (ord);
        if (cmb)
        {
            int chefs = cmb->getChefsNumber();
           
            int CN = cmb->getCN();
            int CS = cmb->getCS();
            if (Free_CN.getcount() >= CN && Free_CS.getcount() >= CS)
            {
                Chefs** Assignedchefs = new Chefs * [chefs];
                PENDING_COMBO.dequeue(ord, pri);
                cmb = dynamic_cast<ComboOrders*> (ord);
                Chefs* c = nullptr;
                int i = 0;
                int cn = CN;
               
                
                while (!Free_CN.isEmpty() && cn > 0 && i < chefs)
                {
                    Free_CN.dequeue(c);
                    Assignedchefs[i++] = c;
                    cn--;
                }
                
                c = nullptr;
                while (!Free_CS.isEmpty() && CS> 0 && i < chefs)
                {
                    Free_CS.dequeue(c);
                    Assignedchefs[i++] = c;
                    CS--;
                }
                cmb->setAssignedChefs(Assignedchefs);
                cmb->setTA(currentTimestep);
                cmb->setAssignedChef(Assignedchefs[0]);
                Cnspeed = (CN > 0) ? Assignedchefs[0]->getSpeed() : 0;
                Assignedchefs[i - 1]->getSpeed();
                cmb->setAVGcooking(Cnspeed, Csspeed);
                Cooking_Orders.enqueue(cmb, cmb->getCookingpriority( )); 
                assigned = true;

            }
            else break;
        
        }
        else break;
    }
    return assigned;

}
bool Restaurant::moveComboToready(int currentTimestep , Orders*& pOrd)
{
    int pri = 0;
    ComboOrders* cmb = dynamic_cast<ComboOrders*>(pOrd);
    if (cmb && currentTimestep - pOrd->getTA() >= cmb->getCookingOrderDuration())
    {
        if (cmb) {
            int idx = 0;
            Chefs** Assignedchefs = cmb->getAssignedChefs();

            for (int i = 0; i < cmb->getCN(); i++)
            {
                Assignedchefs[idx]->setFinishTime(currentTimestep);
                Free_CN.enqueue(Assignedchefs[idx++]);

            }

            for (int i = 0; i < cmb->getCS(); i++)
            {
                Assignedchefs[idx]->setFinishTime(currentTimestep);
                Free_CS.enqueue(Assignedchefs[idx++]);

            }
            delete[] Assignedchefs;
            cmb->setAssignedChefs(nullptr);
            return true;
        }
        else return false;
    }
    else return false;
}


void Restaurant::handleScooterBreakdown(int currentTimestep) {
    
    PriorityQueue<RescueEvent*> tempPending;
    RescueEvent* evt = nullptr;
    int pri;

    while (pendingRescues.dequeue(evt, pri)) {
        if (currentTimestep >= evt->arrivalTimestep) {
            
            evt->failedScooter->setstart_time(evt->breakdownTimestep); 
            evt->failedScooter->setfinish_time(currentTimestep);       
            Back_Scooters.enqueue(evt->failedScooter, evt->failedScooter->getBackPriority());
            delete evt;
        }
        else {
            tempPending.enqueue(evt, pri);
        }
    }
    while (tempPending.dequeue(evt, pri)) pendingRescues.enqueue(evt, pri);

  
    PriorityQueue<Orders*> tempInServ;
    Orders* pOrd = nullptr;

    while (InServ_Orders.dequeue(pOrd, pri)) {
        Deliveryorders* delv = dynamic_cast<Deliveryorders*>(pOrd);

        if (delv && delv->getAssignedScooter() != nullptr && delv->getAssignedScooter()->getType() == SC_TYPE::TYPE_NORMAL) 
        {
            if ((rand() % 100) < 25) {
                Scooters* failedSc = delv->getAssignedScooter();
                Scooters* rescueSc = nullptr;

                if (Resc_Scooters.dequeue(rescueSc)) {
                    int traveledTime = currentTimestep - failedSc->get_StartTime();
                    int rescueArrival = currentTimestep + traveledTime;

                    rescueSc->setstart_time(currentTimestep);
                    rescueSc->updateTotalDistance(delv->getDistance());
                    rescueSc->setTripdistance(delv->getDistance());
                    delv->setAssignedScooter(rescueSc);

                    failedSc->set_Breakdown(true);
                    RescueEvent* newEvt = new RescueEvent();
                    newEvt->failedScooter = failedSc;
                    newEvt->rescueScooter = rescueSc;
                    newEvt->order = delv;
                    newEvt->breakdownTimestep = currentTimestep;
                    newEvt->arrivalTimestep = rescueArrival;

                    pendingRescues.enqueue(newEvt, rescueArrival);
                }
            }
        }
        tempInServ.enqueue(pOrd, pri);
    }
    while (tempInServ.dequeue(pOrd, pri)) InServ_Orders.enqueue(pOrd, pri);
}




////////////////////////// Main simulation Function ////////////////////////////////////

void Restaurant::RunSimulator()
{
	//Set App mode
	Mode = ui.GetMode();

    // reads input file , initialize the restaurant , move to action list
    loadInputFile(); 

    // first print all restaurant parameters (Interactive Mode Only)
    if (Mode == 0) {
        ui.PrintCurrentState(
            0,
            Request, Cancel,
            PEND_ODG, PEND_ODN, PEND_OT,
            PEND_OVN, PEND_OVC, PEND_OVG, PENDING_COMBO,
            Free_CS, Free_CN,
            READY_OD, READY_OT, READY_OV, Overwait_OVG, READY_COMBO,
            Cooking_Orders, InServ_Orders,
            Finished_Orders, Canceled_Orders,
            Free_Scooters, Back_Scooters,
            Maint_Scooters, Resc_Scooters,
            Free_Tables, Busy_Sharable,
            Busy_NonSharable
        );
    }
    // main loop 
    int currentTimestep = 1; 
    while (true)
    {
        ///loop on action lists 
        executeActions(currentTimestep);

		/// Handle Scooter Breakdowns
		handleScooterBreakdown(currentTimestep);
    
        /// check Scooters (Back , Maint) ->free
        checkScootersAvailablity(currentTimestep);

        /// Check finished orders 
		checkFinishedOrders(currentTimestep); 

        /// Assign pending to Chef (syntax to be changed )
        AssignPendingToChef(currentTimestep);

        ///Move cooking to ready
        MoveToReady(currentTimestep);

        ///Check Overwait OVG Orders
        checkOverwaitOVG(currentTimestep);
 
        ///finalize takeaway orders 
        finalizeTakeawayOrders(currentTimestep);

        /// Assign Ready orders
		MovetoInservice(currentTimestep); 

        /// print current stats
        if (Mode == 0) {
            ui.PrintCurrentState(
                currentTimestep,
                Request, Cancel,
                PEND_ODG, PEND_ODN, PEND_OT,
                PEND_OVN, PEND_OVC, PEND_OVG,PENDING_COMBO,
                Free_CS, Free_CN,
                READY_OD, READY_OT, READY_OV, Overwait_OVG, READY_COMBO,
                Cooking_Orders, InServ_Orders,
                Finished_Orders, Canceled_Orders,
                Free_Scooters, Back_Scooters,
                Maint_Scooters, Resc_Scooters,
                Free_Tables, Busy_Sharable,
                Busy_NonSharable
            );
        }
  
        /// Check if simulation ends
        int pending = PEND_ODG.getcount() + PEND_ODN.getcount() + PEND_OT.getcount() + PEND_OVN.getcount() + PEND_OVC.getcount() + PEND_OVG.getcount() + Request.getcount() + Cancel.getcount() + PENDING_COMBO.getcount();
        int active = Cooking_Orders.getcount() + READY_OD.getcount() + READY_OT.getcount() + READY_OV.getcount() + Overwait_OVG.getcount() + InServ_Orders.getcount() + READY_COMBO.getcount() + Back_Scooters.getcount() + Maint_Scooters.getcount();
        if (pending == 0 && active == 0) break;
        currentTimestep++;
    }
    /// Generate output file 
    generateOutputFile(currentTimestep);
    /// print final status
    ui.simulation_ended(currentTimestep, Finished_Orders.getcount(), Canceled_Orders.getcount());
}