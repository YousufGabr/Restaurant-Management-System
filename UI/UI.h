#pragma once
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/ArrayStack.h"
#include "../ADTs/Linked_list.h"
#include "../ADTs/PriorityQueue.h"

class UI
{ 
	public:

		template <typename T>
		void print_stack(ArrayStack<T> S)
		{
			T temp;
			while (S.pop(temp)) cout << temp << " ";
		}

		template <typename T>
		void print_queue(LinkedQueue<T> Q)
		{
			T temp;
			while (Q.dequeue(temp)) cout << temp << " ";
		}

		template <typename T>
		void print_pqueue(PriorityQueue<T> PQ)
		{
			T temp;
			while (PQ.dequeue(temp)) cout << temp << " ";
		}

		template <typename T>
		void print_list(LinkedList<T> L)
		{
			Node<T>* temp = L.getHead();
			while (temp) {
				cout << temp->getItem() << " ";
				temp = temp->getNext();
			}
		}
};

