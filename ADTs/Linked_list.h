#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"
#include <iostream>
using namespace std;

template <typename T>
class LinkedList {
private:
    Node<T>* Head;

    // Helper: Finds node at a specific 1-based position
    Node<T>* GetNodeAt(int pos) {
        if (pos < 1) return nullptr;
        Node<T>* temp = Head;
        int currentPos = 1;
        while (temp && currentPos < pos) {
            temp = temp->getNext();
            currentPos++;
        }
        return temp;
    }

public:
    // --- Constructors & Destructor ---
    
    LinkedList() : Head(nullptr) {}

    LinkedList(const LinkedList& other) {
        Head = nullptr;
        Node<T>* temp = other.Head;
        while (temp) {
            InsertEnd(temp->getItem());
            temp = temp->getNext();
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            DeleteAll();
            Node<T>* temp = other.Head;
            while (temp) {
                InsertEnd(temp->getItem());
                temp = temp->getNext();
            }
        }
        return *this;
    }

    ~LinkedList() {
        DeleteAll();
    }

    // --- Basic Getters & Utilities ---

    Node<T>* getHead() const { return Head; }

    void PrintList() const {
        cout << "\nPrinting list contents:\n\n";
        Node<T>* p = Head;
        while (p) {
            cout << "[ " << p->getItem() << " ]" << "--->";
            p = p->getNext();
        }
        cout << "NULL\n";
    }

    void DeleteAll() {
        Node<T>* P = Head;
        while (Head) {
            P = Head->getNext();
            delete Head;
            Head = P;
        }
    }

    // --- Insertion Methods ---

    void InsertBeg(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        newNode->setNext(Head);
        Head = newNode;
    }

    void InsertEnd(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (!Head) {
            Head = newNode;
            return;
        }
        Node<T>* temp = Head;
        while (temp->getNext()) {
            temp = temp->getNext();
        }
        temp->setNext(newNode);
    }

    bool InsertSorted(const T& data) {
        if (Find(data)) return false;

        if (!Head || data < Head->getItem()) {
            InsertBeg(data);
            return true;
        }

        Node<T>* prv = Head;
        Node<T>* cur = Head->getNext();
        while (cur && cur->getItem() < data) {
            prv = cur;
            cur = cur->getNext();
        }
        Node<T>* newNode = new Node<T>(data);
        newNode->setNext(cur);
        prv->setNext(newNode);
        return true;
    }

    // --- Search & Counting ---

    bool Find(T Key) {
        Node<T>* temp = Head;
        while (temp) {
            if (temp->getItem() == Key) return true;
            temp = temp->getNext();
        }
        return false;
    }

    int CountOccurance(const T& value) {
        int counter = 0;
        Node<T>* temp = Head;
        while (temp) {
            if (temp->getItem() == value) counter++;
            temp = temp->getNext();
        }
        return counter;
    }

    // --- Deletion Methods ---

    void DeleteFirst() {
        if (!Head) return;
        Node<T>* temp = Head;
        Head = Head->getNext();
        delete temp;
    }

    void DeleteLast() {
        if (!Head) return;
        if (!Head->getNext()) {
            DeleteFirst();
            return;
        }
        Node<T>* prv = Head;
        while (prv->getNext()->getNext())
            prv = prv->getNext();

        delete prv->getNext();
        prv->setNext(nullptr);
    }

    bool DeleteNode(const T& value) {
        if (!Head) return false;
        if (Head->getItem() == value) {
            DeleteFirst();
            return true;
        }
        Node<T>* prv = Head;
        Node<T>* cur = Head->getNext();
        while (cur) {
            if (cur->getItem() == value) {
                prv->setNext(cur->getNext());
                delete cur;
                return true;
            }
            prv = cur;
            cur = cur->getNext();
        }
        return false;
    }

    bool DeleteNodes(const T& value) {
        if (!Head) return false;
        bool found = false;

        while (Head && Head->getItem() == value) {
            DeleteFirst();
            found = true;
        }

        Node<T>* cur = Head;
        while (cur && cur->getNext()) {
            if (cur->getNext()->getItem() == value) {
                Node<T>* toDelete = cur->getNext();
                cur->setNext(toDelete->getNext());
                delete toDelete;
                found = true;
            } else {
                cur = cur->getNext();
            }
        }
        return found;
    }

    Node<T>* RemoveMin() {
        if (!Head) return nullptr;
        Node<T>* minNode = Head;
        Node<T>* minPrev = nullptr;
        Node<T>* prv = Head;
        Node<T>* cur = Head->getNext();

        while (cur) {
            if (cur->getItem() < minNode->getItem()) {
                minNode = cur;
                minPrev = prv;
            }
            prv = cur;
            cur = cur->getNext();
        }

        if (minNode == Head) Head = Head->getNext();
        else minPrev->setNext(minNode->getNext());

        minNode->setNext(nullptr);
        return minNode;
    }

    // --- Advanced Operations ---

    void Merge(const LinkedList& L) {
        if (!Head) {
            Head = L.Head;
            return;
        }
        Node<T>* temp = Head;
        while (temp->getNext())
            temp = temp->getNext();
        temp->setNext(L.Head);
    }

    void Reverse() {
        Node<T>* prv = nullptr;
        Node<T>* cur = Head;
        Node<T>* nxt = nullptr;
        while (cur) {
            nxt = cur->getNext();
            cur->setNext(prv);
            prv = cur;
            cur = nxt;
        }
        Head = prv;
    }

    void RemoveDuplicates() {
        Node<T>* cur = Head;
        while (cur) {
            Node<T>* runner = cur;
            while (runner->getNext()) {
                if (runner->getNext()->getItem() == cur->getItem()) {
                    Node<T>* duplicate = runner->getNext();
                    runner->setNext(duplicate->getNext());
                    delete duplicate;
                } else {
                    runner = runner->getNext();
                }
            }
            cur = cur->getNext();
        }
    }

    void SignSplit(LinkedList<T>& Lneg, LinkedList<T>& Lpos) {
        Node<T>* cur = Head;
        while (cur) {
            Node<T>* nextNode = cur->getNext();
            if (cur->getItem() < 0) Lneg.InsertEnd(cur->getItem());
            else if (cur->getItem() > 0) Lpos.InsertEnd(cur->getItem());
            cur = nextNode;
        }
        DeleteAll(); // Clearing current list as items are moved
    }

    void ShiftLargest() {
        if (!Head || !Head->getNext()) return;
        Node<T>* cur = Head;
        while (cur->getNext()) {
            Node<T>* nxt = cur->getNext();
            if (cur->getItem() > nxt->getItem()) {
                T tempVal = cur->getItem();
                cur->setItem(nxt->getItem());
                nxt->setItem(tempVal);
            }
            cur = cur->getNext();
        }
    }

    void MakeDictionary() const {
        Node<T>* current = Head;
        cout << "Dictionary: ";
        while (current) {
            T value = current->getItem();
            bool alreadyPrinted = false;
            Node<T>* checkPrev = Head;
            while (checkPrev != current) {
                if (checkPrev->getItem() == value) {
                    alreadyPrinted = true;
                    break;
                }
                checkPrev = checkPrev->getNext();
            }
            if (!alreadyPrinted) {
                cout << "{" << value << ", " << CountOccurance(value) << "}";
                if (current->getNext()) cout << " -> ";
            }
            current = current->getNext();
        }
        cout << " NULL" << endl;
    }
};

#endif
