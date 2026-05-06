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
    LinkedQueue<Scooters*>& Resc_Scooters,

    // Tables
    BestFitPriQueue& Free_Tables,
    BestFitPriQueue& Busy_Sharable,
    BestFitPriQueue& Busy_NonSharable
)
{
    cout << BOLD << YELLOW << "\n===============================================" << RESET << endl;
    cout << BOLD << YELLOW << "  Current Timestep: " << WHITE << timestep << RESET << endl;
    cout << BOLD << YELLOW << "===============================================" << RESET << endl << endl;

    cout << BOLD << CYAN << "=============== Actions List [Order Type/X, TQ/Tcancel, order ID] ================" << RESET << endl;
    cout << GREEN << Request.getcount() << RESET << " Request actions: "; print_queue(Request); cout << endl << endl;
    cout << GREEN << Cancel.getcount() << RESET << " Cancel actions: ";  print_queue(Cancel);  cout << endl << endl;

    cout << BOLD << CYAN << "------------- Pending Orders IDs ---------------------" << RESET << endl;
    cout << GREEN << PEND_ODG.getcount() << RESET << " ODG: ";   print_queue(PEND_ODG);       cout << endl;
    cout << GREEN << PEND_ODN.getcount() << RESET << " ODN: ";   print_queue(PEND_ODN);       cout << endl << endl;
    cout << GREEN << PEND_OT.getcount() << RESET << " OT:  ";   print_queue(PEND_OT);        cout << endl << endl;
    cout << GREEN << PEND_OVN.getcount() << RESET << " OVN: ";   print_queue(PEND_OVN);       cout << endl;
    cout << GREEN << PEND_OVC.getcount() << RESET << " OVC: ";   print_queue(PEND_OVC);       cout << endl;
    cout << GREEN << PEND_OVG.getcount() << RESET << " OVG: ";   print_pqueue(PEND_OVG);      cout << endl << endl;
    cout << GREEN << PENDING_COMBO.getcount() << RESET << " COMBO: "; print_pqueue(PENDING_COMBO); cout << endl << endl;

    cout << BOLD << CYAN << "------------- Available chefs IDs ----------------------" << RESET << endl;
    cout << GREEN << Free_CS.getcount() << RESET << " CS : "; print_queue(Free_CS); cout << endl;
    cout << GREEN << Free_CN.getcount() << RESET << " CN : "; print_queue(Free_CN); cout << endl << endl;

    cout << BOLD << CYAN << "------------- Cooking orders [Orders ID, chef ID /(#CN,#CS)] ---------------------" << RESET << endl;
    cout << GREEN << Cooking_Orders.getcount() << RESET << " cooking orders: "; print_pqueue(Cooking_Orders); cout << endl << endl;

    cout << BOLD << CYAN << "------------- Ready Orders IDs ---------------------" << RESET << endl;
    cout << GREEN << READY_OD.getcount() << RESET << " OD: ";           print_queue(READY_OD);       cout << endl;
    cout << GREEN << READY_OT.getcount() << RESET << " OT: ";           print_queue(READY_OT);       cout << endl;
    cout << GREEN << READY_OV.getcount() << RESET << " OV: ";           print_queue(READY_OV);       cout << endl;
    cout << GREEN << Overwait_OVG.getcount() << RESET << " Overwait OVG: "; print_pqueue(Overwait_OVG);  cout << endl << endl;
    cout << GREEN << READY_COMBO.getcount() << RESET << " COMBO: ";        print_queue(READY_COMBO);    cout << endl;

    cout << BOLD << CYAN << "------------- Available scooters IDs ----------------------" << RESET << endl;
    cout << GREEN << Free_Scooters.getcount() << RESET << " Normal : "; print_pqueue(Free_Scooters); cout << endl;
    cout << GREEN << Resc_Scooters.getcount() << RESET << " Rescue : "; print_queue(Resc_Scooters);  cout << endl << endl;

    cout << BOLD << CYAN << "------------- Available tables [ID, capacity, free seats] ----------------------" << RESET << endl;
    cout << GREEN << Free_Tables.getcount() << RESET << " Free Tables : "; print_pqueue(Free_Tables); cout << endl;
    cout << GREEN << Busy_Sharable.getcount() << RESET << " Busy Sharable Tables : "; print_pqueue(Busy_Sharable); cout << endl << endl;

    cout << BOLD << CYAN << "------------- In-Service orders [order ID, scooter ID/Table ID/#Scooters] ------------------" << RESET << endl;
    cout << GREEN << InServ_Orders.getcount() << RESET << " Orders: "; print_pqueue(InServ_Orders); cout << endl << endl;

    cout << BOLD << CYAN << "------------- In-maintainance scooters IDs ----------------------" << RESET << endl;
    cout << GREEN << Maint_Scooters.getcount() << RESET << " scooters: "; print_queue(Maint_Scooters); cout << endl << endl;

    cout << BOLD << CYAN << "------------- Scooters Back to Restaurant IDs ----------------------" << RESET << endl;
    cout << GREEN << Back_Scooters.getcount() << RESET << " scooters: "; print_pqueue(Back_Scooters); cout << endl << endl;

    cout << BOLD << CYAN << "------------- Cancelled Orders IDs ----------------------" << RESET << endl;
    cout << GREEN << Canceled_Orders.getcount() << RESET << " cancelled: "; print_stack(Canceled_Orders); cout << endl << endl;

    cout << BOLD << CYAN << "------------- Finished orders IDs----------------------------" << RESET << endl;
    cout << GREEN << Finished_Orders.getcount() << RESET << " Orders: "; print_stack(Finished_Orders); cout << endl << endl;

    cout << BOLD << YELLOW << "\nPRESS ANY KEY TO MOVE TO NEXT STEP !" << RESET << endl;
    cin.get();
}

string UI::getinputfilename() const
{
    cout << "Enter input file name: ";
    string filename;
    cin >> filename;
    cin.ignore();
    return "Input_Files/" + filename + ".txt";
}

string UI::getoutputfilename() const
{
    cout << "Enter output file name: ";
    string filename;
    cin >> filename;
    return "Output_Files/" + filename + ".txt";
}

int UI::GetMode() {
    string input;
    int mode = -1;

    while (true) {
        cout << BOLD << YELLOW << "Please select the application mode:" << RESET << endl;
        cout << GREEN << "[I]" << WHITE << " Interactive Mode" << RESET << endl;
        cout << GREEN << "[S]" << WHITE << " Silent Mode" << RESET << endl;
        cout << BOLD << YELLOW << "Choice: " << RESET;

        cin >> input;
        cin.ignore();

        char choice = toupper(input[0]);

        if (choice == 'I') {
            mode = 0;
            cout << GREEN << "Interactive Mode selected." << RESET << endl;
            break;
        }
        else if (choice == 'S') {
            mode = 1;
            cout << GREEN << "Silent Mode selected." << RESET << endl;
            break;
        }
        else {
            cout << RED << "---------------------------------------" << RESET << endl;
            cout << RED << "Invalid choice. Please enter 'S' or 'I'." << RESET << endl;
            cout << RED << "---------------------------------------" << RESET << endl;
        }
    }
    return mode;
}

void UI::simulation_ended(int timestep, int finishedorders, int canceledorders)
{
    cout << BOLD << YELLOW << "\nSIMULATION ENDED AT TIMESTEP: " << WHITE << timestep << RESET << endl;
    cout << GREEN << "Total Finished Orders: " << WHITE << finishedorders << RESET << endl;
    cout << RED << "Total Canceled Orders: " << WHITE << canceledorders << RESET << endl;
    char input;
    cout << YELLOW << "Type [X] then Enter to EXIT..." << RESET << endl;

    do {
        input = cin.get();
    } while (toupper(input) != 'X');
}
void UI::welcome_msg()
{
    cout << BOLD << YELLOW << "\n*******************************************" << RESET << endl;
    cout << BOLD << YELLOW << "*                                         *" << RESET << endl;
    cout << BOLD << YELLOW << "*       WELCOME TO Our Restaurant         *" << RESET << endl;
    cout << BOLD << YELLOW << "*                                         *" << RESET << endl;
    cout << BOLD << YELLOW << "*******************************************" << RESET << endl;
    cout << GREEN << "\n  Serving delicious code since 2026!\n" << RESET << endl;
}