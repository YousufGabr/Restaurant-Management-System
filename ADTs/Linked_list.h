#define _LINKEDLIST

#include "Node.h"



template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
	Node<T>* Getnodeat(int pos) {
		if (pos < 1) return nullptr;
		Node<T>* temp = Head;
		while (temp) {
			pos--;
			if (pos == 0) return temp;
			temp = temp->getNext();
		}
		return nullptr;
	}
public:
	Node <T>* getHead() { return Head; }

	LinkedList(const LinkedList& other) {
		Head = nullptr;
		Node<T>* temp = other.Head;
		while (temp) {
			InsertEnd(temp->getItem());
			temp = temp->getNext();
		}
	}
	LinkedList& operator=(const LinkedList& other) {
		if (this == &other)
			return *this;

		DeleteAll();
		Head = nullptr;

		Node<T>* temp = other.Head;
		while (temp) {
			InsertEnd(temp->getItem());
			temp = temp->getNext();
		}

		return *this;
	}
	LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()	const
	{
		cout << "\nprinting list contents:\n\n";
		Node<T>* p = Head;

		while (p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T& data)
	{
		Node<T>* temp = new Node<T>(data);
		temp->setNext(Head);
		Head = temp;

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T& data) {
		Node <T>* temp = Head;
		Node<T>* newNode = new Node<T>(data);
		if (!Head) { Head = newNode; return; }
		while (temp->getNext()) {
			temp = temp->getNext();
		}

		temp->setNext(newNode);
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key) {
		Node <T>* temp = Head;
		while (temp) {
			if (temp->getItem() == Key) return true;
			temp = temp->getNext();
		}
		return false;
	}


	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value) {
		int counter = 0;
		Node <T>* temp = Head;
		while (temp) {
			if (temp->getItem() == value) counter++;
			temp = temp->getNext();
		}
		return counter;
	}

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst() {
		if (!Head) return;
		Node<T>* temp = Head;
		Head = Head->getNext();
		delete temp;
	}


	//[5] DeleteLast
	//Deletes the last node in the list
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

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T& value) {
		if (!Head) return false;

		if (Head->getItem() == value) {
			DeleteFirst();
			return true;
		}

		Node<T>* temp = Head->getNext();
		Node<T>* prv = Head;

		while (temp) {
			if (temp->getItem() == value) {
				prv->setNext(temp->getNext());
				delete temp;
				return true;
			}
			prv = temp;
			temp = temp->getNext();
		}
		return false;
	}


	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted

	bool DeleteNodes(const T& value) {
		if (!Head) return false;
		bool found = false;


		while (Head && Head->getItem() == value) {
			DeleteFirst();
			found = true;
		}

		// 2. „⁄«·Ã… »«ﬁÌ «·ﬁ«∆„…
		Node<T>* cur = Head;
		while (cur && cur->getNext()) {
			if (cur->getNext()->getItem() == value) {
				Node<T>* toDelete = cur->getNext();
				cur->setNext(toDelete->getNext());
				delete toDelete;
				found = true;
				// ·« ‰Õ—ﬂ cur Â‰« ·√‰‰« ‰Õ «Ã ·›Õ’ «·‰Êœ «·ÃœÌœ… «· Ì √’»Õ  cur->next
			}
			else {
				cur = cur->getNext();
			}
		}
		return found;
	}

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
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

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse() {
		Node<T>* prv = nullptr;
		Node<T>* cur = Head;
		Node<T>* nxt = nullptr;
		while (cur != nullptr) {
			nxt = cur->getNext();
			cur->setNext(prv);
			prv = cur;
			cur = nxt;
		}
		Head = prv;
	}
	void PrintKth(int k) {
		if (k <= 0) { cout << "Beyond List Length" << endl; return; }
		Node<T>* temp = Head;
		int count = 1;
		while (temp != nullptr) {
			if (count == k) {
				cout << temp->getItem() << endl;
				return;
			}
			temp = temp->getNext();
			count++;
		}
		cout << "Beyond List Length" << endl;
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
	LinkedList<T> CloneList() {
		LinkedList<T> newList;
		if (!Head) return newList;

		Node<T>* temp = Head;
		while (temp) {
			newList.InsertEnd(temp->getItem());
			temp = temp->getNext();
		}
		return newList;
	}
	void SignSplit(LinkedList<T>& Lneg, LinkedList<T>& Lpos) {
		Node<T>* cur = Head;
		Node<T>* prv = nullptr;

		while (cur) {
			if (cur->getItem() < 0) {
				Lneg.InsertEnd(cur->getItem());
				Node<T>* toDelete = cur;
				if (cur == Head) { Head = Head->getNext(); cur = Head; }
				else { prv->setNext(cur->getNext()); cur = prv->getNext(); }
				delete toDelete;
			}
			else if (cur->getItem() > 0) {
				Lpos.InsertEnd(cur->getItem());
				Node<T>* toDelete = cur;
				if (cur == Head) { Head = Head->getNext(); cur = Head; }
				else { prv->setNext(cur->getNext()); cur = prv->getNext(); }
				delete toDelete;
			}
			else {
				prv = cur;
				cur = cur->getNext();
			}
		}
	}
	LinkedList<T> MergeSorted(LinkedList<T>& L2) {
		LinkedList<T> mergedList;
		Node<T>* p1 = Head;
		Node<T>* p2 = L2.Head;

		while (p1 || p2) {
			if (p1 && (!p2 || p1->getItem() <= p2->getItem())) {
				mergedList.InsertEnd(p1->getItem());
				p1 = p1->getNext();
			}
			else {
				mergedList.InsertEnd(p2->getItem());
				p2 = p2->getNext();
			}
		}
		return mergedList;
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
				}
				else {
					runner = runner->getNext();
				}
			}
			cur = cur->getNext();
		}
	}
	LinkedList<T>* SumLists(const LinkedList<T>& L1, const LinkedList<T>& L2) {
		Node<T>* temp1 = L1.Head;
		Node<T>* temp2 = L2.Head;

		Node<T>* check1 = temp1;
		Node<T>* check2 = temp2;
		while (check1 && check2) {
			check1 = check1->getNext();
			check2 = check2->getNext();
		}
		if (check1 || check2) return nullptr;

		LinkedList<T>* resultList = new LinkedList<T>();
		while (temp1 && temp2) {
			T sumValue = temp1->getItem() + temp2->getItem();
			resultList->InsertEnd(sumValue);
			temp1 = temp1->getNext();
			temp2 = temp2->getNext();
		}
		return resultList;
	}

	void Reorder_X(T X) {
		if (!Head) return;

		LinkedList<T> smallerEqual;
		LinkedList<T> larger;

		Node<T>* cur = Head;
		while (cur) {
			if (cur->getItem() <= X)
				smallerEqual.InsertEnd(cur->getItem());
			else
				larger.InsertEnd(cur->getItem());
			cur = cur->getNext();
		}

		this->DeleteAll();
		Node<T>* temp = smallerEqual.Head;
		while (temp) {
			this->InsertEnd(temp->getItem());
			temp = temp->getNext();
		}
		temp = larger.Head;
		while (temp) {
			this->InsertEnd(temp->getItem());
			temp = temp->getNext();
		}
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

		cout << "Dictionary:";

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
				int count = 0;
				Node<T>* runner = Head;
				while (runner != nullptr) {
					if (runner->getItem() == value) {
						count++;
					}
					runner = runner->getNext();
				}
				cout << "{" << value << ", " << count << "}";
				Node<T>* nextUnique = current->getNext();

				if (nextUnique != nullptr) cout << " -> ";
			}

			current = current->getNext();
		}
		cout << " NULL" << endl;
	}
	bool remove2(int pos) {
		if (pos < 1) return false;
		Node<T>* target = Getnodeat(pos);
		Node<T>* prv = Getnodeat(pos - 1);
		prv->setNext(target->getNext());
		delete target;
		return true;
	}
	bool remove3(int pos) {

		DeleteNode(Getnodeat(pos)->getItem());
		return true;
	}


	Node<T>* reverseRecursive(Node<T>* curr, Node<T>* prev = nullptr) {

		if (!curr) {
			Head = prev;
			return prev;
		}
		Node<T>* nxt = curr->getNext();
		curr->setNext(prev);
		return reverseRecursive(nxt, curr);
	}
	void PrintLots(LinkedList <T>* L, LinkedList <T>* P) {
		Node <T>* p1 = P->getHead();
		Node <T>* l1 = L->getHead();
		int count = 0;
		while (p1) {

			for (int i = count; i < p1->getItem(); i++) {
				if (l1->getNext()) l1 = l1->getNext();
				else return;
				count++;
			}
			if (l1) cout << l1->getItem() << " ";
			else return;
			p1 = p1->getNext();
		}
		cout << endl;
	}
};