#pragma once
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/ArrayStack.h"
#include "../ADTs/PriorityQueue.h"

class UI
{ 
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
};

