#pragma once
#include "../Actions/Actions.h"
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

class Restaurant
{
private:
	struct CancelEvent {
		int cancelTime;
		int orderID;
	};

	
	UI ui;
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

	//Chefs
	LinkedQueue<Chefs*> Free_CS; 
	LinkedQueue<Chefs*> Free_CN; 

	//Ready Orders
	LinkedQueue<Orders*> READY_OD; 
	LinkedQueue<Orders*> READY_OT;  
	CancelQueue READY_OV;  

	//Order Status
	CancelPriQueue Cooking_Orders; 
	PriorityQueue<Orders*> InServ_Orders; 
	ArrayStack<Orders*> Finished_Orders; 
	ArrayStack<Orders*> Canceled_Orders; 

	//Scooters
	PriorityQueue<Scooters*> Free_Scooters;
	PriorityQueue<Scooters*> Back_Scooters; 
	LinkedQueue<Scooters*> Maint_Scooters; 

	//Tables
	BestFitPriQueue Free_Tables; 
	BestFitPriQueue Busy_Sharable;
	BestFitPriQueue Busy_NonSharable; 

public:
	Restaurant();

	
	void GenerateRandomOrders();
	void CancelOVC(int orderID);
	void RunPhase1Simulator();
	void loadInputFile(string filename);
	void generateOutputFile(std::string filename);
	void AssignPendingToChef(int currentTimestep);
	void finalizeTakeawayOrders(int currentTimestep);

};

