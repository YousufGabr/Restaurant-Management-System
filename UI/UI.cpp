#include "UI.h"

template<typename T>
void UI::print_stack(ArrayStack<T>& S)
{
	S.print();
}

template<typename T>
void UI::print_queue(LinkedQueue<T>& Q)
{
	Q.print();
}

template<typename T>
void UI::print_pqueue(PriorityQueue<T>& PQ)
{
	PQ.print();
}
