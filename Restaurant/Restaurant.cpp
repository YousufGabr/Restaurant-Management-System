// ============================================================
//  Restaurant.cpp – Phase 1.2 Final Stabilized Version
// ============================================================

#include "Restaurant.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
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
        Free_Scooters.enqueue(S, S->getPriority());
    }

    // Initialize Tables
    int caps[] = { 3,3,4,4,5,5,5,5,5,6,6,6,7,7,7,7,7,8,8,8 };
    for (int i = 0; i < 20; i++)
    {
        Tables* t = new Tables(i + 1, caps[i]);
        Free_Tables.enqueue(t, t->getPriority());
    }
}

void Restaurant::AddOrder(Orders* ord)
{
    if (!ord) return;
    switch (ord->getType())
    {
    case TYPE_ODG: PEND_ODG.enqueue(ord); break;
    case TYPE_ODN: PEND_ODN.enqueue(ord); break;
    case TYPE_OT:  PEND_OT.enqueue(ord);  break;
    case TYPE_OVN: PEND_OVN.enqueue(ord); break;
    case TYPE_OVC: PEND_OVC.enqueue(ord, (int)(ord->getPriority() * 1000)); break;
    case TYPE_OVG: PEND_OVG.enqueue(ord, (int)(ord->getPriority() * 1000)); break;
    }
}

void Restaurant::CancelOVC(int orderID)
{
    Orders* canceled = PEND_OVC.CancelOrder(orderID);
    if (canceled) { Canceled_Orders.push(canceled); return; }

    canceled = READY_OV.CancelOrder(orderID);
    if (canceled) { Canceled_Orders.push(canceled); return; }

    LinkedQueue<Orders*> tempQ;
    Orders* ord = nullptr;
    while (Cooking_Orders.dequeue(ord))
    {
        if (ord->getID() == orderID) canceled = ord;
        else tempQ.enqueue(ord);
    }
    while (tempQ.dequeue(ord)) Cooking_Orders.enqueue(ord);

    if (canceled)
    {
        Chefs* chf = canceled->getAssignedChef();
        if (chf)
        {
            chf->makeAvailable();
            if (chf->getType() == Chefs::TYPE_CS) Free_CS.enqueue(chf);
            else Free_CN.enqueue(chf);
            canceled->setAssignedChef(nullptr);
        }
        Canceled_Orders.push(canceled);
    }
}

void Restaurant::GenerateRandomOrders()
{
    for (int i = 1; i <= 500; ++i)
    {
        int    orderType = rand() % 6;
        int    id = i;
        int    size = (rand() % 10) + 1;
        double price = (rand() % 400) + 100.0;

        Orders* newOrder = new Orders((ORD_TYPE)orderType, 1, id, size, price);

        if (orderType == 0 || orderType == 1)
            newOrder->setDineInInfo((rand() % 5) + 2, (rand() % 30) + 10, rand() % 2 == 0);
        else if (orderType >= 3)
            newOrder->setDeliveryDistance((rand() % 4000) + 500);

        AddOrder(newOrder);
    }
}

void Restaurant::PrintCurrentState(int timestep)
{
    cout << "\nCurrent Timestep: " << timestep << endl;
    cout << "=============== Actions List ================" << endl;
    cout << "0 actions remaining" << endl;

    cout << "------------- Pending Orders IDs ---------------------" << endl;
    cout << PEND_ODG.getcount() << " ODG: "; ui.print_queue(PEND_ODG);  cout << endl;
    cout << PEND_ODN.getcount() << " ODN: "; ui.print_queue(PEND_ODN);  cout << endl;
    cout << PEND_OT.getcount() << " OT:  "; ui.print_queue(PEND_OT);   cout << endl;
    cout << PEND_OVN.getcount() << " OVN: "; ui.print_queue(PEND_OVN);  cout << endl;
    cout << PEND_OVC.getcount() << " OVC: "; ui.print_pqueue(PEND_OVC); cout << endl;
    cout << PEND_OVG.getcount() << " OVG: "; ui.print_pqueue(PEND_OVG); cout << endl;

    cout << "------------- Available chefs IDs ----------------------" << endl;
    cout << Free_CS.getcount() << " CS : "; ui.print_queue(Free_CS); cout << endl;
    cout << Free_CN.getcount() << " CN : "; ui.print_queue(Free_CN); cout << endl;

    cout << "------------- Cooking orders [Orders ID, chef ID] ---------------------" << endl;
    cout << Cooking_Orders.getcount() << " cooking orders: "; ui.print_queue(Cooking_Orders); cout << endl;

    cout << "------------- Ready Orders IDs ---------------------" << endl;
    cout << READY_OD.getcount() << " OD: "; ui.print_queue(READY_OD);  cout << endl;
    cout << READY_OT.getcount() << " OT: "; ui.print_queue(READY_OT);  cout << endl;
    cout << READY_OV.getcount() << " OV: "; ui.print_pqueue(READY_OV); cout << endl;

    cout << "------------- Available scooters IDs ----------------------" << endl;
    cout << Free_Scooters.getcount() << " Scooters : "; ui.print_pqueue(Free_Scooters); cout << endl;

    cout << "------------- Available tables [ID, capacity, free seats] ----------------------" << endl;
    cout << Free_Tables.getcount() << " tables : "; ui.print_pqueue(Free_Tables); cout << endl;

    cout << "------------- In-Service orders [order ID, scooter/Table ID] ------------------" << endl;
    cout << InServ_Orders.getcount() << " Orders: "; ui.print_queue(InServ_Orders); cout << endl;

    cout << "------------- In-maintainance scooters IDs ----------------------" << endl;
    cout << Maint_Scooters.getcount() << " scooters: "; ui.print_queue(Maint_Scooters); cout << endl;

    cout << "------------- Scooters Back to Restaurant IDs ----------------------" << endl;
    cout << Back_Scooters.getcount() << " scooters: "; ui.print_pqueue(Back_Scooters); cout << endl;

    cout << "------------- Cancelled Orders IDs ----------------------" << endl;
    cout << Canceled_Orders.getcount() << " cancelled: "; ui.print_stack(Canceled_Orders); cout << endl;

    cout << "------------- Finished orders IDs----------------------------" << endl;
    cout << Finished_Orders.getcount() << " Orders: "; ui.print_stack(Finished_Orders); cout << endl;

    cout << "\nPRESS ANY KEY TO MOVE TO NEXT STEP !" << endl;
    cin.get();
}

void Restaurant::RunPhase1Simulator()
{
    GenerateRandomOrders();
    int timestep = 1;

    while (true)
    {
        // 3.1: Move Pending to Cooking
        for (int i = 0; i < 30; i++)
        {
            if (Free_CS.isEmpty() && Free_CN.isEmpty()) break;

            int nonEmpty = 0;
            if (!PEND_ODG.isEmpty()) nonEmpty++; if (!PEND_ODN.isEmpty()) nonEmpty++;
            if (!PEND_OT.isEmpty())  nonEmpty++; if (!PEND_OVN.isEmpty()) nonEmpty++;
            if (!PEND_OVC.isEmpty()) nonEmpty++; if (!PEND_OVG.isEmpty()) nonEmpty++;
            if (nonEmpty == 0) break;

            Orders* ord = nullptr; int p = 0;
            int choice = rand() % nonEmpty; int idx = 0;
            if (!PEND_ODG.isEmpty() && idx++ == choice) PEND_ODG.dequeue(ord);
            else if (!PEND_ODN.isEmpty() && idx++ == choice) PEND_ODN.dequeue(ord);
            else if (!PEND_OT.isEmpty() && idx++ == choice) PEND_OT.dequeue(ord);
            else if (!PEND_OVN.isEmpty() && idx++ == choice) PEND_OVN.dequeue(ord);
            else if (!PEND_OVC.isEmpty() && idx++ == choice) PEND_OVC.dequeue(ord, p);
            else if (!PEND_OVG.isEmpty() && idx++ == choice) PEND_OVG.dequeue(ord, p);

            if (!ord) continue;

            Chefs* chf = nullptr;
            if (!Free_CS.isEmpty() && (Free_CN.isEmpty() || rand() % 2 == 0)) Free_CS.dequeue(chf);
            else Free_CN.dequeue(chf);

            ord->setTA(timestep);
            ord->setAssignedChef(chf);
            Cooking_Orders.enqueue(ord);
        }

        // 3.2: Move Cooking to Ready
        for (int i = 0; i < 15; i++)
        {
            if (Cooking_Orders.isEmpty()) break;
            if ((rand() % 100) < 75)
            {
                Orders* ord = nullptr; Cooking_Orders.dequeue(ord);
                if (ord) {
                    Chefs* chf = ord->getAssignedChef();
                    if (chf) {
                        if (chf->getType() == Chefs::TYPE_CS) Free_CS.enqueue(chf);
                        else Free_CN.enqueue(chf);
                    }
                    ord->setTR(timestep);
                    if (ord->getType() == TYPE_ODG || ord->getType() == TYPE_ODN) READY_OD.enqueue(ord);
                    else if (ord->getType() == TYPE_OT) READY_OT.enqueue(ord);
                    else READY_OV.enqueue(ord, (int)(ord->getPriority() * 1000));
                }
            }
        }

        // 3.3: Move Ready to In-Service 
        for (int i = 0; i < 10; i++)
        {
            Orders* ord = nullptr; int p = 0;
            if (!READY_OT.isEmpty()) {
                READY_OT.dequeue(ord);
                ord->setTS(timestep); ord->setTF(timestep + 1);
                Finished_Orders.push(ord);
            }
            else if (!READY_OD.isEmpty() && !Free_Tables.isEmpty()) {
                READY_OD.dequeue(ord);
                Tables* tbl = Free_Tables.getBest(ord);
                if (tbl) {
                    tbl->assign_order(ord, timestep);
                    Busy_NonSharable.enqueue(tbl, tbl->getPriority());
                    ord->setTS(timestep); InServ_Orders.enqueue(ord);
                }
            }
            else if (!READY_OV.isEmpty() && !Free_Scooters.isEmpty()) {
                READY_OV.dequeue(ord, p);
                Scooters* sc = nullptr; Free_Scooters.dequeue(sc, p);
                if (sc) {
                    // ==========================================================
                    // FIXED LOGIC: Update the scooter's total distance correctly
                    // ==========================================================
                    sc->setTotalDistance(ord->getDistance());

                    sc->assignOrder(ord, timestep);
                    ord->setAssignedScooter(sc);
                    ord->setTS(timestep); InServ_Orders.enqueue(ord);
                }
            }
        }

        // 3.7: Service Finish - Scooter MUST go to Back_Scooters FIRST
        if ((rand() % 100) < 25 && !InServ_Orders.isEmpty())
        {
            Orders* ord = nullptr; InServ_Orders.dequeue(ord);
            if (ord) {
                ord->setTF(timestep); Finished_Orders.push(ord);
                if (ord->getType() >= 3) { // Delivery
                    Scooters* sc = ord->getAssignedScooter();
                    if (sc) {
                        // Priority gets recalculated here automatically because of the updated distance!
                        Back_Scooters.enqueue(sc, sc->getPriority());
                        ord->setAssignedScooter(nullptr);
                    }
                }
                else if (ord->getType() < 2) { // Dine-in
                    Tables* tbl = nullptr; int pri = 0; PriorityQueue<Tables*> tempPQ;
                    bool found = false;
                    while (Busy_NonSharable.dequeue(tbl, pri)) {
                        if (!found && tbl->is_free(timestep)) { Free_Tables.enqueue(tbl, tbl->getPriority()); found = true; }
                        else tempPQ.enqueue(tbl, pri);
                    }
                    while (tempPQ.dequeue(tbl, pri)) Busy_NonSharable.enqueue(tbl, pri);
                }
            }
        }

        // 3.8: Returning Scooters (Back -> Maint/Free)
        if (!Back_Scooters.isEmpty() && (rand() % 100 < 50)) {
            int p = 0;
            Scooters* sc = nullptr; Back_Scooters.dequeue(sc, p);
            // After being in "Back", it either becomes available or needs maintenance
            // 'p' holds the newly updated priority, so re-enqueueing works perfectly.
            if (rand() % 2 == 0) Free_Scooters.enqueue(sc, p);
            else Maint_Scooters.enqueue(sc);
        }

        // 3.9: Maintenance (Maint -> Free)
        if (!Maint_Scooters.isEmpty() && (rand() % 100 < 50)) {
            Scooters* sc = nullptr; Maint_Scooters.dequeue(sc);
            Free_Scooters.enqueue(sc, sc->getPriority());
        }

        PrintCurrentState(timestep);

        int pending = PEND_ODG.getcount() + PEND_ODN.getcount() + PEND_OT.getcount() + PEND_OVN.getcount() + PEND_OVC.getcount() + PEND_OVG.getcount();
        int active = Cooking_Orders.getcount() + READY_OD.getcount() + READY_OT.getcount() + READY_OV.getcount() + InServ_Orders.getcount() + Back_Scooters.getcount() + Maint_Scooters.getcount();
        if (pending == 0 && active == 0) break;
        timestep++;
    }
}