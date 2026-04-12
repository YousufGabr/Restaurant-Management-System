#pragma once
#include "priNode.h"
#include <iostream>
using namespace std;

//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class PriorityQueue
{
    priNode<T>* head;
    int count;
public:
    PriorityQueue() : head(nullptr) , count(0){}

    ~PriorityQueue() {
        T tmp;
        int p;
        while (dequeue(tmp,p));
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {
            
            newNode->setNext(head);
            head = newNode;
            count++;
            return;
        }
       
        priNode<T>* current = head;        
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext( current->getNext());
        current->setNext( newNode);   
        count++;
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        count--;
        return true;
    }
    // to be revised
    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;
        int PRI = 0;
        topEntry = head->getItem(PRI);
        pri = PRI;
        return true;
    }

    int getcount() { return count;}

    bool isEmpty() const {
        return head == nullptr;
    }

    void print() const
    {
        priNode<T>* current = head;
        while (current)
        {
            int pri = 0;
            cout << current->getItem(pri) << " (Priority: " << pri << ") ";
            current = current->getNext();
        }
    }
};
