// Restaurant.cpp  –  Phase 1.2  Random Simulator
// Paste this file as-is.  It includes every function declared in Restaurant.h

#include "Restaurant.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// ============================================================
//  Constructor
//  Pre-populate chefs, scooters, and tables so the random
//  simulator has resources to work with from the start.
// ============================================================
Restaurant::Restaurant()
{
    srand((unsigned)time(0));

    // 10 Special Chefs (CS), speed = 2 dishes/timestep
    for (int i = 1; i <= 10; i++)
        Free_CS.enqueue(new Chefs(i, Chefs::TYPE_CS, 2));

    // 25 Normal Chefs (CN), speed = 3 dishes/timestep
    for (int i = 11; i <= 35; i++)
        Free_CN.enqueue(new Chefs(i, Chefs::TYPE_CN, 3));

    // 20 Scooters: speed=150 m/ts, maintenance duration=23, threshold=8 orders
    for (int i = 1; i <= 20; i++)
        Free_Scooters.enqueue(new Scooters(i, 150, 23, 8));

    // 20 Tables with mixed capacities (priority = capacity for best-fit)
    int caps[] = { 3,3,4,4,5,5,5,5,5,6,6,6,7,7,7,7,7,8,8,8 };
    for (int i = 0; i < 20; i++)
    {
        Tables* t = new Tables(i + 1, caps[i]);
        Free_Tables.enqueue(t, caps[i]);
    }
}

// ============================================================
//  AddOrder
//  Routes a newly created order pointer to the correct
//  pending list.  Called by RequestAction::Act() in Phase 2.
// ============================================================
void Restaurant::AddOrder(Orders* ord)
{
    if (!ord) return;
    switch (ord->getType())
    {
    case TYPE_ODG: PEND_ODG.enqueue(ord);                          break;
    case TYPE_ODN: PEND_ODN.enqueue(ord);                          break;
    case TYPE_OT:  PEND_OT.enqueue(ord);                           break;
    case TYPE_OVN: PEND_OVN.enqueue(ord);                          break;
    case TYPE_OVC: PEND_OVC.enqueue(ord, (int)ord->getPriority()); break;
    case TYPE_OVG: PEND_OVG.enqueue(ord, (int)ord->getPriority()); break;
    }
}

// ============================================================
//  CancelOVC
//  Cancels an OVC order from wherever it currently lives:
//    1. pending OVC list
//    2. ready OV list
//    3. cooking list  (also releases the assigned chef)
//  Called by CancelAction::Act() in Phase 2.
// ============================================================
void Restaurant::CancelOVC(int orderID)
{
    // 1. Pending OVC
    Orders* canceled = PEND_OVC.CancelOrder(orderID);
    if (canceled) { Canceled_Orders.push(canceled); return; }

    // 2. Ready OV
    canceled = READY_OV.CancelOrder(orderID);
    if (canceled) { Canceled_Orders.push(canceled); return; }

    // 3. Cooking list – manual search
    LinkedQueue<Orders*> tempQ;
    Orders* ord = nullptr;
    while (Cooking_Orders.dequeue(ord))
    {
        if (ord->getID() == orderID)
            canceled = ord;
        else
            tempQ.enqueue(ord);
    }
    while (tempQ.dequeue(ord))
        Cooking_Orders.enqueue(ord);

    if (canceled)
    {
        Chefs* chf = canceled->getAssignedChef();
        if (chf)
        {
            chf->makeAvailable();
            if (chf->getType() == Chefs::TYPE_CS) Free_CS.enqueue(chf);
            else                                   Free_CN.enqueue(chf);
            canceled->setAssignedChef(nullptr);
        }
        Canceled_Orders.push(canceled);
    }
}

// ============================================================
//  GenerateRandomOrders
//  Creates at least 500 random orders and distributes them
//  across the six pending lists.
// ============================================================
void Restaurant::GenerateRandomOrders()
{
    for (int i = 1; i <= 500; ++i)
    {
        int    orderType = rand() % 6;
        int    tq = 1;
        int    id = i;
        int    size = (rand() % 10) + 1;
        double price = (rand() % 400) + 100.0;

        Orders* newOrder = nullptr;

        switch (orderType)
        {
        case 0: // ODG – Dine-in Grilled
            newOrder = new Orders(TYPE_ODG, tq, id, size, price);
            newOrder->setDineInInfo((rand() % 5) + 2, (rand() % 30) + 10, rand() % 2 == 0);
            PEND_ODG.enqueue(newOrder);
            break;

        case 1: // ODN – Dine-in Normal
            newOrder = new Orders(TYPE_ODN, tq, id, size, price);
            newOrder->setDineInInfo((rand() % 5) + 2, (rand() % 30) + 10, rand() % 2 == 0);
            PEND_ODN.enqueue(newOrder);
            break;

        case 2: // OT – Takeaway
            newOrder = new Orders(TYPE_OT, tq, id, size, price);
            PEND_OT.enqueue(newOrder);
            break;

        case 3: // OVC – Delivery Cold
            newOrder = new Orders(TYPE_OVC, tq, id, size, price);
            newOrder->setDeliveryDistance((rand() % 4000) + 500);
            PEND_OVC.enqueue(newOrder, (int)newOrder->getPriority());
            break;

        case 4: // OVG – Delivery Grilled
            newOrder = new Orders(TYPE_OVG, tq, id, size, price);
            newOrder->setDeliveryDistance((rand() % 4000) + 500);
            PEND_OVG.enqueue(newOrder, (int)newOrder->getPriority());
            break;

        case 5: // OVN – Delivery Normal
            newOrder = new Orders(TYPE_OVN, tq, id, size, price);
            newOrder->setDeliveryDistance((rand() % 4000) + 500);
            PEND_OVN.enqueue(newOrder);
            break;
        }
    }
}

// ============================================================
//  PrintCurrentState  (Step 3.10)
//  Prints every list in the exact sequence from the
//  "Program Interface" section of the project description.
// ============================================================
void Restaurant::PrintCurrentState(int timestep)
{
    cout << "\nCurrent Timestep: " << timestep << endl;

    // Actions list – no input file in Phase 1
    cout << "=============== Actions List ================" << endl;
    cout << "0 actions remaining" << endl;

    // Pending Orders
    cout << "------------- Pending Orders IDs ---------------------" << endl;
    cout << PEND_ODG.getcount() << " ODG: "; ui.print_queue(PEND_ODG);  cout << endl;
    cout << PEND_ODN.getcount() << " ODN: "; ui.print_queue(PEND_ODN);  cout << endl;
    cout << PEND_OT.getcount() << " OT:  "; ui.print_queue(PEND_OT);   cout << endl;
    cout << PEND_OVN.getcount() << " OVN: "; ui.print_queue(PEND_OVN);  cout << endl;
    cout << PEND_OVC.getcount() << " OVC: "; ui.print_pqueue(PEND_OVC); cout << endl;
    cout << PEND_OVG.getcount() << " OVG: "; ui.print_pqueue(PEND_OVG); cout << endl;

    // Available Chefs
    cout << "------------- Available chefs IDs ----------------------" << endl;
    cout << Free_CS.getcount() << " CS : "; ui.print_queue(Free_CS); cout << endl;
    cout << Free_CN.getcount() << " CN : "; ui.print_queue(Free_CN); cout << endl;

    // Cooking Orders
    cout << "------------- Cooking orders [Orders ID, chef ID] ---------------------" << endl;
    cout << Cooking_Orders.getcount() << " cooking orders: ";
    ui.print_queue(Cooking_Orders);
    cout << endl;

    // Ready Orders
    cout << "------------- Ready Orders IDs ---------------------" << endl;
    cout << READY_OD.getcount() << " OD: "; ui.print_queue(READY_OD);  cout << endl;
    cout << READY_OT.getcount() << " OT: "; ui.print_queue(READY_OT);  cout << endl;
    cout << READY_OV.getcount() << " OV: "; ui.print_pqueue(READY_OV); cout << endl;

    // Available Scooters
    cout << "------------- Available scooters IDs ----------------------" << endl;
    cout << Free_Scooters.getcount() << " Scooters : ";
    ui.print_queue(Free_Scooters);
    cout << endl;

    // Available Tables
    cout << "------------- Available tables [ID, capacity, free seats] ----------------------" << endl;
    cout << Free_Tables.getcount() << " tables : ";
    ui.print_pqueue(Free_Tables);
    cout << endl;

    // In-Service Orders
    cout << "------------- In-Service orders [order ID, scooter/Table ID] ------------------" << endl;
    cout << InServ_Orders.getcount() << " Orders: ";
    ui.print_queue(InServ_Orders);
    cout << endl;

    // In-Maintenance Scooters
    cout << "------------- In-maintainance scooters IDs ----------------------" << endl;
    cout << Maint_Scooters.getcount() << " scooters: ";
    ui.print_queue(Maint_Scooters);
    cout << endl;

    // Scooters Back to Restaurant
    cout << "------------- Scooters Back to Restaurant IDs ----------------------" << endl;
    cout << Back_Scooters.getcount() << " scooters: ";
    ui.print_queue(Back_Scooters);
    cout << endl;

    // Cancelled Orders
    cout << "------------- Cancelled Orders IDs ----------------------" << endl;
    cout << Canceled_Orders.getcount() << " cancelled: ";
    ui.print_stack(Canceled_Orders);
    cout << endl;

    // Finished Orders – stack prints top first (most recently finished first)
    cout << "------------- Finished orders IDs----------------------------" << endl;
    cout << Finished_Orders.getcount() << " Orders: ";
    ui.print_stack(Finished_Orders);
    cout << endl;

    cout << "\nPRESS ANY KEY TO MOVE TO NEXT STEP !" << endl;
    cin.get();
}

// ============================================================
//  RunPhase1Simulator
//  Main simulation loop.  Follows steps 3.1 – 3.10 exactly
//  as described in the Phase 1.2 specification.
// ============================================================
void Restaurant::RunPhase1Simulator()
{
    GenerateRandomOrders();

    int timestep = 1;

    while (true)
    {
        // ============================================================
        // STEP 3.1 – Repeat EXACTLY 30 times (no probability):
        //   Randomly pick the top order from one of the pending lists.
        //   Pick a random chef and move that order to the Cooking list.
        // ============================================================
        for (int i = 0; i < 30; i++)
        {
            // Count non-empty pending lists
            int nonEmpty = 0;
            if (!PEND_ODG.isEmpty()) nonEmpty++;
            if (!PEND_ODN.isEmpty()) nonEmpty++;
            if (!PEND_OT.isEmpty())  nonEmpty++;
            if (!PEND_OVN.isEmpty()) nonEmpty++;
            if (!PEND_OVC.isEmpty()) nonEmpty++;
            if (!PEND_OVG.isEmpty()) nonEmpty++;

            if (nonEmpty == 0) break; // no pending orders – skip rest of 30

            // Randomly pick one non-empty list and dequeue its front order
            Orders* ord = nullptr;
            int     choice = rand() % nonEmpty;
            int     idx = 0;

            if (!PEND_ODG.isEmpty() && idx++ == choice) PEND_ODG.dequeue(ord);
            if (!PEND_ODN.isEmpty() && idx++ == choice) PEND_ODN.dequeue(ord);
            if (!PEND_OT.isEmpty() && idx++ == choice) PEND_OT.dequeue(ord);
            if (!PEND_OVN.isEmpty() && idx++ == choice) PEND_OVN.dequeue(ord);
            if (!PEND_OVC.isEmpty() && idx++ == choice) { int p = 0; PEND_OVC.dequeue(ord, p); }
            if (!PEND_OVG.isEmpty() && idx++ == choice) { int p = 0; PEND_OVG.dequeue(ord, p); }

            if (!ord) continue;

            // Count available chef pools
            int chefPools = 0;
            if (!Free_CS.isEmpty()) chefPools++;
            if (!Free_CN.isEmpty()) chefPools++;

            if (chefPools == 0)
            {
                // No chef available – return order to its list and try next iteration
                AddOrder(ord);
                continue;
            }

            // Randomly pick a chef from whichever pools are available
            Chefs* chf = nullptr;
            int    chefChoice = rand() % chefPools;
            int    chefIdx = 0;

            if (!Free_CS.isEmpty() && chefIdx++ == chefChoice) Free_CS.dequeue(chf);
            if (!Free_CN.isEmpty() && chefIdx++ == chefChoice) Free_CN.dequeue(chf);

            // Set timing, assign chef, move order to Cooking list
            ord->setTA(timestep);
            ord->setAssignedChef(chf);
            int cookTime = (ord->getSize() + chf->getSpeed() - 1) / chf->getSpeed();
            chf->setFinishTime(timestep + cookTime);
            Cooking_Orders.enqueue(ord);
        }

        // ============================================================
        // STEP 3.2 – Repeat 15 times:
        //   With 75% probability: pick top order from Cooking list,
        //   release its chef, move to the correct ready list.
        // ============================================================
        for (int i = 0; i < 15; i++)
        {
            if (Cooking_Orders.isEmpty()) break;

            if ((rand() % 100) < 75)
            {
                Orders* ord = nullptr;
                Cooking_Orders.dequeue(ord);
                if (!ord) continue;

                // Release the chef
                Chefs* chf = ord->getAssignedChef();
                if (chf)
                {
                    chf->makeAvailable();
                    if (chf->getType() == Chefs::TYPE_CS) Free_CS.enqueue(chf);
                    else                                   Free_CN.enqueue(chf);
                    ord->setAssignedChef(nullptr);
                }
                ord->setTR(timestep);

                // Route to correct ready list based on order type
                ORD_TYPE t = ord->getType();
                if (t == TYPE_ODG || t == TYPE_ODN) READY_OD.enqueue(ord);
                else if (t == TYPE_OT)                   READY_OT.enqueue(ord);
                else                                     READY_OV.enqueue(ord, (int)ord->getPriority());
            }
        }

        // ============================================================
        // STEP 3.3 – Repeat EXACTLY 10 times (no probability):
        //   Randomly pick the top order from one of the ready lists.
        //   OT → Finish list
        //   OV → pick random scooter → In-service list
        //   OD → pick random table   → In-service list
        // ============================================================
        for (int i = 0; i < 10; i++)
        {
            int nonEmpty = 0;
            if (!READY_OD.isEmpty()) nonEmpty++;
            if (!READY_OT.isEmpty()) nonEmpty++;
            if (!READY_OV.isEmpty()) nonEmpty++;

            if (nonEmpty == 0) break;

            Orders* ord = nullptr;
            int     choice = rand() % nonEmpty;
            int     idx = 0;

            if (!READY_OD.isEmpty() && idx++ == choice) READY_OD.dequeue(ord);
            if (!READY_OT.isEmpty() && idx++ == choice) READY_OT.dequeue(ord);
            if (!READY_OV.isEmpty() && idx++ == choice) { int p = 0; READY_OV.dequeue(ord, p); }

            if (!ord) continue;

            ord->setTS(timestep);
            ORD_TYPE t = ord->getType();

            if (t == TYPE_OT)
            {
                // Takeaway: customer picks up → straight to Finish
                ord->setTF(timestep + 1);
                Finished_Orders.push(ord);
            }
            else if (t == TYPE_ODG || t == TYPE_ODN)
            {
                // Dine-in: assign a table (best-fit) → In-service
                if (!Free_Tables.isEmpty())
                {
                    Tables* tbl = Free_Tables.getBest(ord);
                    if (tbl)
                    {
                        tbl->assign_order(ord, timestep);
                        Busy_NonSharable.enqueue(tbl, tbl->get_free_Seats());
                    }
                }
                InServ_Orders.enqueue(ord);
            }
            else // OVC, OVG, OVN – delivery
            {
                // Assign a scooter → In-service
                if (!Free_Scooters.isEmpty())
                {
                    Scooters* sc = nullptr;
                    Free_Scooters.dequeue(sc);
                    if (sc) sc->assignOrder(ord, timestep);
                }
                InServ_Orders.enqueue(ord);
            }
        }

        // ============================================================
        // STEP 3.4 – Generate random ID, call CancelOrder on PEND_OVC.
        //   If found → Cancelled list.
        // ============================================================
        {
            int     randID = (rand() % 500) + 1;
            Orders* canceled = PEND_OVC.CancelOrder(randID);
            if (canceled) Canceled_Orders.push(canceled);
        }

        // ============================================================
        // STEP 3.5 – Generate random ID, call CancelOrder on READY_OV.
        //   If found → Cancelled list.
        // ============================================================
        {
            int     randID = (rand() % 500) + 1;
            Orders* canceled = READY_OV.CancelOrder(randID);
            if (canceled) Canceled_Orders.push(canceled);
        }

        // ============================================================
        // STEP 3.6 – Generate random ID, search Cooking list for OV.
        //   If found: remove, release chef → Cancelled list.
        // ============================================================
        {
            int randID = (rand() % 500) + 1;

            LinkedQueue<Orders*> tempQ;
            Orders* ord = nullptr;
            Orders* canceled = nullptr;

            while (Cooking_Orders.dequeue(ord))
            {
                ORD_TYPE t = ord->getType();
                bool     isOV = (t == TYPE_OVC || t == TYPE_OVG || t == TYPE_OVN);

                if (!canceled && isOV && ord->getID() == randID)
                    canceled = ord;   // found – do not re-enqueue
                else
                    tempQ.enqueue(ord);
            }
            // Restore cooking list
            while (tempQ.dequeue(ord))
                Cooking_Orders.enqueue(ord);

            if (canceled)
            {
                Chefs* chf = canceled->getAssignedChef();
                if (chf)
                {
                    chf->makeAvailable();
                    if (chf->getType() == Chefs::TYPE_CS) Free_CS.enqueue(chf);
                    else                                   Free_CN.enqueue(chf);
                    canceled->setAssignedChef(nullptr);
                }
                Canceled_Orders.push(canceled);
            }
        }

        // ============================================================
        // STEP 3.7 – With 25% probability:
        //   Pick top order from In-service list → Finish list.
        //   If OV → scooter goes to Scooter_Back list.
        //   If OD → table goes back to Free_Tables list.
        // ============================================================
        if ((rand() % 100) < 25 && !InServ_Orders.isEmpty())
        {
            Orders* ord = nullptr;
            InServ_Orders.dequeue(ord);
            if (ord)
            {
                ord->setTF(timestep);
                Finished_Orders.push(ord);

                ORD_TYPE t = ord->getType();

                if (t == TYPE_OVC || t == TYPE_OVG || t == TYPE_OVN)
                {
                    // OV: scooter returns → Scooter_Back
                    // Phase 1 proxy: move one free scooter to Back to represent the returning scooter
                    if (!Free_Scooters.isEmpty())
                    {
                        Scooters* sc = nullptr;
                        Free_Scooters.dequeue(sc);
                        if (sc) Back_Scooters.enqueue(sc);
                    }
                }
                else if (t == TYPE_ODG || t == TYPE_ODN)
                {
                    // OD: find the finished table in Busy and return it to Free_Tables
                    Tables* tbl = nullptr;
                    int     pri = 0;
                    PriorityQueue<Tables*> tempPQ;
                    bool found = false;

                    while (Busy_NonSharable.dequeue(tbl, pri))
                    {
                        if (!found && tbl->is_free(timestep))
                        {
                            Free_Tables.enqueue(tbl, tbl->get_capacity());
                            found = true;
                        }
                        else
                            tempPQ.enqueue(tbl, pri);
                    }
                    while (tempPQ.dequeue(tbl, pri))
                        Busy_NonSharable.enqueue(tbl, pri);
                }
            }
        }

        // ============================================================
        // STEP 3.8 – With 50% probability:
        //   Pick scooter from Scooter_Back → randomly Free or Maintenance.
        // ============================================================
        if ((rand() % 100) < 50 && !Back_Scooters.isEmpty())
        {
            Scooters* sc = nullptr;
            Back_Scooters.dequeue(sc);
            if (sc)
            {
                if (rand() % 2 == 0) Free_Scooters.enqueue(sc);
                else                  Maint_Scooters.enqueue(sc);
            }
        }

        // ============================================================
        // STEP 3.9 – With 50% probability:
        //   Pick scooter from Maintenance list → Free Scooters list.
        // ============================================================
        if ((rand() % 100) < 50 && !Maint_Scooters.isEmpty())
        {
            Scooters* sc = nullptr;
            Maint_Scooters.dequeue(sc);
            if (sc) Free_Scooters.enqueue(sc);
        }

        // ============================================================
        // STEP 3.10 – Print info of all lists (except actions)
        // ============================================================
        PrintCurrentState(timestep);

        // ============================================================
        // STEP 4 – Terminate when ALL orders are in Finish or Cancel
        // ============================================================
        int pending = PEND_ODG.getcount() + PEND_ODN.getcount() + PEND_OT.getcount()
            + PEND_OVN.getcount() + PEND_OVC.getcount() + PEND_OVG.getcount();

        int active = Cooking_Orders.getcount()
            + READY_OD.getcount() + READY_OT.getcount() + READY_OV.getcount()
            + InServ_Orders.getcount();

        if (pending == 0 && active == 0) break;

        timestep++;
    }

    cout << "\n===== SIMULATION COMPLETE =====" << endl;
    cout << "Total Finished : " << Finished_Orders.getcount() << endl;
    cout << "Total Cancelled: " << Canceled_Orders.getcount() << endl;
}