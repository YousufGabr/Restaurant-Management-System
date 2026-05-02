#include "UI.h"

void UI::PrintCurrentState(
    int timestep,
    // Actions
    LinkedQueue<Actions*>& Request,
    LinkedQueue<Actions*>& Cancel,

    // Pending Orders
    LinkedQueue<Orders*>& PEND_ODG,
    LinkedQueue<Orders*>& PEND_ODN,
    LinkedQueue<Orders*>& PEND_OT,
    LinkedQueue<Orders*>& PEND_OVN,
    CancelQueue& PEND_OVC,
    PriorityQueue<Orders*>& PEND_OVG,
    PriorityQueue<Orders*>& PENDING_COMBO,


    // Chefs
    LinkedQueue<Chefs*>& Free_CS,
    LinkedQueue<Chefs*>& Free_CN,

    // Ready Orders
    LinkedQueue<Orders*>& READY_OD,
    LinkedQueue<Orders*>& READY_OT,
    CancelQueue& READY_OV,
    PriorityQueue<Orders*>& Overwait_OVG,
    LinkedQueue<Orders*>& READY_COMBO,

    // Order Status
    CancelPriQueue& Cooking_Orders,
    PriorityQueue<Orders*>& InServ_Orders,
    ArrayStack<Orders*>& Finished_Orders,
    ArrayStack<Orders*>& Canceled_Orders,

    // Scooters
    PriorityQueue<Scooters*>& Free_Scooters,
    PriorityQueue<Scooters*>& Back_Scooters,
    LinkedQueue<Scooters*>& Maint_Scooters,

    // Tables
    BestFitPriQueue& Free_Tables,
    BestFitPriQueue& Busy_Sharable,
    BestFitPriQueue& Busy_NonSharable
)
{
    cout << "\nCurrent Timestep: " << timestep << endl << endl;
    cout << "=============== Actions List [Order Type/X, TQ/Tcancel, order ID] ================" << endl;
    cout << Request.getcount() << " Request actions: "; print_queue(Request); cout << endl << endl;
    cout << Cancel.getcount() << " Cancel actions: "; print_queue(Cancel); cout << endl << endl;
    
    cout << "------------- Pending Orders IDs ---------------------" << endl;
    cout << PEND_ODG.getcount() << " ODG: "; print_queue(PEND_ODG);  cout << endl;
    cout << PEND_ODN.getcount() << " ODN: "; print_queue(PEND_ODN);  cout << endl << endl;
    cout << PEND_OT.getcount() << " OT:  "; print_queue(PEND_OT);   cout << endl << endl;
    cout << PEND_OVN.getcount() << " OVN: "; print_queue(PEND_OVN);  cout << endl;
    cout << PEND_OVC.getcount() << " OVC: "; print_queue(PEND_OVC); cout << endl;
    cout << PEND_OVG.getcount() << " OVG: "; print_pqueue(PEND_OVG); cout << endl << endl;
    cout << PENDING_COMBO.getcount() << " COMBO: "; print_pqueue(PENDING_COMBO); cout << endl << endl;

    cout << "------------- Available chefs IDs ----------------------" << endl;
    cout << Free_CS.getcount() << " CS : "; print_queue(Free_CS); cout << endl;
    cout << Free_CN.getcount() << " CN : "; print_queue(Free_CN); cout << endl << endl;

    cout << "------------- Cooking orders [Orders ID, chef ID] ---------------------" << endl;
    cout << Cooking_Orders.getcount() << " cooking orders: "; print_pqueue(Cooking_Orders); cout << endl << endl;

    cout << "------------- Ready Orders IDs ---------------------" << endl;
    cout << READY_OD.getcount() << " OD: "; print_queue(READY_OD);  cout << endl;
    cout << READY_OT.getcount() << " OT: "; print_queue(READY_OT);  cout << endl;
    cout << READY_OV.getcount() << " OV: "; print_queue(READY_OV); cout << endl;
    cout << Overwait_OVG.getcount() << " Overwait OVG: "; print_pqueue(Overwait_OVG); cout << endl << endl;
    cout << READY_COMBO.getcount() << " COMBO: "; print_queue(READY_COMBO); cout << endl;

    cout << "------------- Available scooters IDs ----------------------" << endl;
    cout << Free_Scooters.getcount() << " Scooters : "; print_pqueue(Free_Scooters); cout << endl << endl;

    cout << "------------- Available tables [ID, capacity, free seats] ----------------------" << endl;
    cout << Free_Tables.getcount() + Busy_Sharable.getcount() << " tables : "; print_pqueue(Busy_Sharable);  print_pqueue(Free_Tables); cout << endl << endl;

    cout << "------------- In-Service orders [order ID, scooter/Table ID] ------------------" << endl;
    cout << InServ_Orders.getcount() << " Orders: "; print_pqueue(InServ_Orders); cout << endl << endl;

    cout << "------------- In-maintainance scooters IDs ----------------------" << endl;
    cout << Maint_Scooters.getcount() << " scooters: "; print_queue(Maint_Scooters); cout << endl << endl;

    cout << "------------- Scooters Back to Restaurant IDs ----------------------" << endl;
    cout << Back_Scooters.getcount() << " scooters: "; print_pqueue(Back_Scooters); cout << endl << endl;

    cout << "------------- Cancelled Orders IDs ----------------------" << endl;
    cout << Canceled_Orders.getcount() << " cancelled: "; print_stack(Canceled_Orders); cout << endl << endl;

    cout << "------------- Finished orders IDs----------------------------" << endl;
    cout << Finished_Orders.getcount() << " Orders: "; print_stack(Finished_Orders); cout << endl << endl;

    cout << "\nPRESS ANY KEY TO MOVE TO NEXT STEP !" << endl;
    cin.get();
}

string UI::getinputfilename() const
{
	cout << "Enter input file name: ";
	string filename;
	cin >> filename;
	cin.ignore();
	return filename + ".txt";
}

string UI::getoutputfilename() const
{
    cout << "Enter output file name: ";
    string filename;
    cin >> filename;
    return filename + ".txt";
}

int UI::GetMode() {
    string input;
    int mode = -1;

    while (true) {
        cout << "Please select the application mode:" << endl;
        cout << "[I] Interactive Mode" << endl;
        cout << "[S] Silent Mode" << endl;
        cout << "Choice: ";

        cin >> input;
        cin.ignore();

        // Convert input to uppercase to handle 's' or 'i'
        char choice = toupper(input[0]);

        if (choice == 'I') {
            mode = 0; 
            cout << "Interactive Mode selected." << endl;
            break;
        }
        else if (choice == 'S') {
            mode = 1;
            cout << "Silent Mode selected." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please enter 'S' or 'I'." << endl;
            cout << "---------------------------------------" << endl;
        }
    }
    return mode;
}

void UI::simulation_ended(int timestep, int finishedorders, int canceledorders)
{
    cout << "\nSIMULATION ENDED AT TIMESTEP: " << timestep << endl;
    cout << "Total Finished Orders: " << finishedorders << endl;
    cout << "Total Canceled Orders: " << canceledorders << endl;
    char input;
    cout << "Type [X] then Enter to EXIT..." << endl;

   
    do {
        input = cin.get();
    } while (toupper(input) != 'X'); 
}