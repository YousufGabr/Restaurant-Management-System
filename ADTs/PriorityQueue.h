#ifndef _PRIORITY_QUEUE
#define _PRIORITY_QUEUE

#include "Node.h" // Assuming your Node class is in Node.h

template <typename T>
class PriorityQueue {
private:
    Node<T>* backPtr;  // Points to the back of the queue
    Node<T>* frontPtr; // Points to the front of the queue

public:
    PriorityQueue() {
        backPtr = nullptr;
        frontPtr = nullptr;
    }

    bool isEmpty() const {
        return (frontPtr == nullptr);
    }

    // Inserts items in a sorted manner (Highest priority at the front)
    bool enqueue(const T& newEntry) {
        Node<T>* newNodePtr = new Node<T>(newEntry);

        // Case 1: Queue is empty or newEntry has higher priority than the current front
        if (isEmpty() || newEntry > frontPtr->getItem()) {
            newNodePtr->setNext(frontPtr);
            frontPtr = newNodePtr;
            return true;
        }

        // Case 2: Traverse the list to find the correct insertion point
        Node<T>* prev = frontPtr;
        Node<T>* curr = frontPtr->getNext();

        while (curr != nullptr && !(newEntry > curr->getItem())) {
            prev = curr;
            curr = curr->getNext();
        }

        // Insert the node between prev and curr
        newNodePtr->setNext(curr);
        prev->setNext(newNodePtr);

        return true;
    }

    // Removes the highest priority item from the front
    bool dequeue(T& frntEntry) {
        if (isEmpty()) return false;

        Node<T>* nodeToDeletePtr = frontPtr;
        frntEntry = frontPtr->getItem();
        frontPtr = frontPtr->getNext();

        // Free memory
        delete nodeToDeletePtr;
        nodeToDeletePtr = nullptr;

        return true;
    }

    // Peek at the highest priority item
    T peek() const {
        if (!isEmpty()) {
            return frontPtr->getItem();
        }
        // Handle empty queue case as needed
        return T();
    }

    ~PriorityQueue() {
        T temp;
        while (dequeue(temp));
    }
};

#endif