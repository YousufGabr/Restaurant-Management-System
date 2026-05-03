#pragma once
#include "../Actions/Actions.h"
#include "../Actions/RequestAction.h"
#include "../Actions/CancelAction.h"
#include "../ADTs/ArrayStack.h"
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/BestFitPriQueue.h"
#include "../ADTs/CancelPriQueue.h"
#include "../Classes/Chefs.h"
#include "../Classes/Tables.h"
#include "../Classes/Orders.h"
#include "../Classes/Scooters.h"
#include "../UI/UI.h"
#include "../ADTs/CancelQueue.h"
#include "../Classes/Deliveryorders.h"
#include "../Classes/Dineorders.h"
#include "../Classes/Takeawayorders.h"
#include"..//Classes/ComboOrders.h"

class Restaurant
{
private:
	struct CancelEvent {
		int cancelTime;
		int orderID;
	};
	struct RescueEvent {
		Scooters* failedScooter;
		Scooters* rescueScooter;
		Deliveryorders* order;
		int breakdownTimestep;
		int arrivalTimestep; // when rescue scooter reaches the failed one
	};

	
	UI ui;
	int Mode; // 0 for interactive, 1 for silent
	//Actions
	LinkedQueue<Actions*> Request; 
	LinkedQueue<Actions*> Cancel;  

	//Pending Orders
	LinkedQueue<Orders*> PEND_ODG;
	LinkedQueue<Orders*> PEND_ODN;
	LinkedQueue<Orders*> PEND_OT;
	LinkedQueue<Orders*> PEND_OVN;
	CancelQueue PEND_OVC;  
	PriorityQueue<Orders*> PEND_OVG;
	PriorityQueue<Orders*> PENDING_COMBO;

	//Chefs
	LinkedQueue<Chefs*> Free_CS; 
	LinkedQueue<Chefs*> Free_CN; 

	//Ready Orders
	LinkedQueue<Orders*> READY_OD;
	LinkedQueue<Orders*> READY_OT;
	CancelQueue READY_OV;  
	LinkedQueue<Orders*> READY_COMBO;
	PriorityQueue<Orders*> Overwait_OVG;

	//Order Status
	CancelPriQueue Cooking_Orders; 
	PriorityQueue<Orders*> InServ_Orders; 
	ArrayStack<Orders*> Finished_Orders; 
	ArrayStack<Orders*> Canceled_Orders; 

	//Scooters
	PriorityQueue<Scooters*> Free_Scooters;
	PriorityQueue<Scooters*> Back_Scooters; 
	LinkedQueue<Scooters*> Maint_Scooters; 
	LinkedQueue<Scooters*> Resc_Scooters;
	PriorityQueue<RescueEvent*> pendingRescues;

	//Tables
	BestFitPriQueue Free_Tables; 
	BestFitPriQueue Busy_Sharable;
	BestFitPriQueue Busy_NonSharable; 

public:
	Restaurant();

	//Actions called functions 
	void AddPendingOrder(Orders *);
	void CancelOVC(int orderID);


	void RunSimulator();
	void executeActions(int currentTimestep);
	void checkScootersAvailablity(int currentTimestep);
	void checkFinishedOrders(int currentTimestep);
	void loadInputFile();
	void generateOutputFile();
	void AssignPendingToChef(int currentTimestep);
	void MoveToReady(int currentTimestep);
	void checkOverwaitOVG(int currentTimestep);
	void MovetoInservice(int currentTimestep);
	void finalizeTakeawayOrders(int currentTimestep);
	void assignComboTochef(int currentTimestep);
	bool moveComboToready(int currentTimestep , Orders*& Pord);
	void handleScooterBreakdown(int currentTimestep);


};

