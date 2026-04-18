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

        switch (orderType)
        {
        case TYPE_ODG: PEND_ODG.enqueue(newOrder); break;
        case TYPE_ODN: PEND_ODN.enqueue(newOrder); break;
        case TYPE_OT:  PEND_OT.enqueue(newOrder);  break;
        case TYPE_OVN: PEND_OVN.enqueue(newOrder); break;
        case TYPE_OVC: PEND_OVC.enqueue(newOrder); break;
        case TYPE_OVG: PEND_OVG.enqueue(newOrder, (int)(newOrder->getPriority() * 1000)); break;
        }
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

void Restaurant::RunPhase1Simulator()
{
    GenerateRandomOrders();
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
}
void Restaurant::loadInputFile(std::string filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) return;

    // 1. قراءة بيانات الطهاة
    int numCN, numCS;
    inputFile >> numCN >> numCS; // [cite: 860]

    int speedCN, speedCS;
    inputFile >> speedCN >> speedCS; // [cite: 861]

    // يمكنك هنا عمل Loop لإنشاء الـ Chefs وإضافتهم للقوائم (Free_CN و Free_CS)

    // 2. قراءة بيانات الدراجات النارية (Scooters)
    int sCount, sSpeed;
    inputFile >> sCount >> sSpeed; // [cite: 862]

    int mainOrds, mainDur;
    inputFile >> mainOrds >> mainDur; // [cite: 863]

    // يمكنك هنا عمل Loop لإنشاء الـ Scooters وإضافتهم لقائمة Free_Scooters

    // 3. قراءة بيانات الطاولات (Tables)
    int totalTables;
    inputFile >> totalTables; // [cite: 864]

    int loadedTables = 0;
    while (loadedTables < totalTables) {
        int count, capacity;
        inputFile >> count >> capacity; // [cite: 866]
        // قم بإنشاء الطاولات بناءً على العدد والسعة وأضفهم لـ Free_Tables
        loadedTables += count;
    }

    // 4. قراءة حد الانتظار الأقصى (Overwait Threshold) 
    int TH;
    inputFile >> TH; // [cite: 867]

    // 5. قراءة الأحداث (Actions)
    int M;
    inputFile >> M; // [cite: 868]

    for (int i = 0; i < M; ++i) { // [cite: 869]
        char actionType;
        inputFile >> actionType;

        if (actionType == 'Q') { // طلب جديد (Request Action) [cite: 870]
            std::string TYP;
            int TQ, ID, SIZE;
            double price;
            inputFile >> TYP >> TQ >> ID >> SIZE >> price; // [cite: 875, 876, 877, 878]

            if (TYP == "ODG" || TYP == "ODN") { // طلبات الأكل داخل المطعم
                int seats, duration;
                char canShareChar;
                inputFile >> seats >> duration >> canShareChar; // [cite: 879, 880, 881]
                bool canShare = (canShareChar == 'Y' || canShareChar == 'y');
                // قم بإنشاء RequestAction الخاص بـ Dine-in وأضفه لطابور الأحداث
            }
            else if (TYP == "OVC" || TYP == "OVG" || TYP == "OVN") { // طلبات التوصيل
                double distance;
                inputFile >> distance; // [cite: 882]
                // قم بإنشاء RequestAction الخاص بـ Delivery وأضفه لطابور الأحداث
            }
            else if (TYP == "OT") { // طلبات الاستلام الذاتي
                // قم بإنشاء RequestAction الخاص بـ Takeaway وأضفه لطابور الأحداث
            }
        }
        else if (actionType == 'X') { // إلغاء طلب (Cancel Action) [cite: 871, 884]
            int Tcancel, ID;
            inputFile >> Tcancel >> ID; // [cite: 885, 886]
            // قم بإنشاء CancelAction وأضفه لطابور الأحداث
        }
    }
    inputFile.close();
}
void Restaurant::generateOutputFile(std::string filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return;

    outFile << "TF  ID  TQ  TA  TR  TS  Ti  Tc  Tw  Tserv\n";

    // 1. طباعة الطلبات المنتهية 
    // يمكنك لاحقاً عمل Loop هنا لطباعة بيانات كل طلب من Finished_Orders
    /*
    Order* ord = nullptr;
    while(Finished_Orders.pop(ord)) {
        outFile << ord->getTF() << " " << ord->getID() << " " ... << "\n";
    }
    */

    outFile << "\n-----------------------------------------------------------\n";
    outFile << "-------------------- Statistics ---------------------------\n";

    // =========================================================================
    // تعريف المتغيرات لتجنب الإيرور (Errors)
    // ملاحظة: هذه القيم حالياً مبدئية، ويجب عليك في Phase 2 حسابها بشكل حقيقي
    // =========================================================================

    int countODG = 0, countODN = 0, countOT = 0, countOVG = 0, countOVN = 0, countOVC = 0;

    int finishedCount = Finished_Orders.getcount(); // سحب العدد من الكود الخاص بك
    int cancelledCount = Canceled_Orders.getcount(); // سحب العدد من الكود الخاص بك
    int totalOrders = finishedCount + cancelledCount;

    int countCN = 0; // قم بحسابهم عند القراءة من الملف
    int countCS = 0;
    int totalChefs = countCN + countCS;

    int totalScooters = 0; // قم بحسابه عند القراءة من الملف
    int overwaitCount = 0; // عدد الطلبات التي تجاوزت وقت الانتظار

    float avgTi = 0.0, avgTc = 0.0, avgTw = 0.0, avgTserv = 0.0;
    float scooterUtilization = 0.0, chefUtilization = 0.0;

    // حماية من القسمة على صفر (Divide by Zero)
    float finishPer = (totalOrders == 0) ? 0 : ((float)finishedCount / totalOrders) * 100;
    float cancelPer = (totalOrders == 0) ? 0 : ((float)cancelledCount / totalOrders) * 100;
    float overwaitPer = (totalOrders == 0) ? 0 : ((float)overwaitCount / totalOrders) * 100;

    // =========================================================================
    // 2. طباعة الإحصائيات في الملف
    // =========================================================================

    // الإحصائية الأولى
    outFile << "1- Total number of orders = " << totalOrders
        << " (ODG: " << countODG << ", ODN: " << countODN
        << ", OT: " << countOT << ", OVG: " << countOVG
        << ", OVN: " << countOVN << ", OVC: " << countOVC << ")\n";

    // الإحصائية الثانية
    outFile << "2- Total number of chefs = " << totalChefs
        << " (CN: " << countCN << ", CS: " << countCS << ")\n";

    // الإحصائية الثالثة
    outFile << "3- Total number of scooters = " << totalScooters << "\n";

    // الإحصائية الرابعة
    outFile << "4- Percentage of finished orders = " << finishPer
        << "% , Percentage of cancelled orders = " << cancelPer << "%\n";

    // الإحصائية الخامسة
    outFile << "5- Percentage of overwait orders = " << overwaitPer << "%\n";

    // الإحصائية السادسة
    outFile << "6- Average Ti = " << avgTi << ", Average Tc = " << avgTc
        << ", Average Tw = " << avgTw << ", Average Tserv = " << avgTserv << "\n";

    // الإحصائية السابعة
    outFile << "7- Scooters utilization % = " << scooterUtilization << "%\n";

    // الإحصائية الثامنة
    outFile << "8- Chefs utilization % = " << chefUtilization << "%\n";

    outFile.close();
}
