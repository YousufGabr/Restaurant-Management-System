#pragma once
#include <iostream>
#include "../Actions/Actions.h"
#include "../ADTs/ArrayStack.h"
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/BestFitPriQueue.h"
#include "../ADTs/CancelPriQueue.h"
#include "../Classes/Chefs.h"
#include "../Classes/Tables.h"
#include "../Classes/Orders.h"
#include "../Classes/Scooters.h"
#include "../ADTs/CancelQueue.h"
#include"../Classes/Deliveryorders.h"
#include"../Classes/Dineorders.h"
#include"../Classes/Takeawayorders.h"
#include "../Classes/ComboOrders.h"

using namespace std;
class Actions;


class UI
{ 
    // ANSI helpers
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"
#define WHITE   "\033[97m"
#define BOLD    "\033[1m"

	public:

		template<typename T>
		void print_stack(ArrayStack<T>& S)
		{
			S.print();
		}

		template<typename T>
		void print_queue(LinkedQueue<T>& Q)
		{
			Q.print();
		}

		template<typename T>
		void print_pqueue(PriorityQueue<T>& PQ)
		{
			PQ.print();
		}
        void PrintCurrentState(
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
        );

		string getinputfilename() const;
		string getoutputfilename() const;
        int GetMode();
        void simulation_ended(int timestep, int finishedorders, int canceledorders);
        void welcome_msg();
};

