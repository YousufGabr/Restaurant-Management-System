#pragma once
#include "../Actions/Actions.h"
#include "../ADTs/ArrayStack.h"
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/BestFitPriQueue.h"
#include "../ADTs/CancelPriQueue.h"
#include "../Classes/chefs.h"
#include "../Classes/tables.h"
#include "../Classes/orders.h"
#include "../Classes/scooters.h"
#include "../UI/UI.h"

class Actions;

class Restaurant
{
private:
	//Action
	LinkedQueue<Actions*> Request; //All Request actions in the input file
	LinkedQueue<Actions*> Cancel;  //All cancel actions in the input file

	//Pending Orders
	LinkedQueue<orders*> PEND_ODG; //All pending Grilled Dine-in orders
	LinkedQueue<orders*> PEND_ODN; //All pending Normal Dine-in orders
	LinkedQueue<orders*> PEND_OT;  //All pending Takeaway orders
	LinkedQueue<orders*> PEND_OVN;  //All pending Normal Delivery orders
	CancelPriQueue PEND_OVC;  //All pending Cold Delivery orders
	PriorityQueue<orders*> PEND_OVG;  //All pending Grilled Delivery orders

	//Chefs
	LinkedQueue<chefs*> Free_CS; //All Free Special Chefs in the restaurant
	LinkedQueue<chefs*> Free_CN; //All Free Normal Chefs in the restaurant

	//Ready Orders
	LinkedQueue<orders*> READY_OD; //All Ready Dine-in orders
	LinkedQueue<orders*> READY_OT;  //All Ready Takeaway orders
	CancelPriQueue READY_OV;  //All Ready Delivery orders

	//Order Status
	LinkedQueue<orders*> Cooking_Orders; //All orders currently being cooked
	LinkedQueue<orders*> InServ_Orders; //All orders currently being served (table/scooter)
	ArrayStack<orders*> Finished_Orders; //All finished orders (served and delivered)
	ArrayStack<orders*> Canceled_Orders; //All canceled orders

	//Scooters
	LinkedQueue<scooters*> Free_Scooters; //All Free Scooters in the restaurant
	LinkedQueue<scooters*> Back_Scooters; //All Scooters currently back from delivery and waiting for maintenance
	LinkedQueue<scooters*> Maint_Scooters; //All Scooters currently in maintenance

	//Tables
	BestFitPriQueue Free_Tables; //All Free Tables in the restaurant
	BestFitPriQueue Busy_Sharable; //All Occupied Sharable Tables in the restaurant
	BestFitPriQueue Busy_NonSharable; //All Occupied Non-Sharable Tables in the restaurant

public:
	Restaurant();


};
