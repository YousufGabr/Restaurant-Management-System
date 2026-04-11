
//  created by Frank M. Carrano and Timothy M. Henry.
// / Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.



#ifndef ARRAY_STACK_
#define ARRAY_STACK_

#include "StackADT.h"

//Unless spesificed by the stack user, the default size is 100
template<typename T>
class ArrayStack : public StackADT<T>
{
private:
	T *items;		// Array of stack items
	int      top;                   // Index to top of stack
	const int STACK_SIZE;
	int count;

public:

	ArrayStack(int MaxSize) : STACK_SIZE(MaxSize)
	{
		items = new T[STACK_SIZE];
		top = -1;
	}  // end default constructor

	//Function getCapacity() returns the stack max size
	//added for array implementaion only
	int getCapacity()	
	{
		return STACK_SIZE;
	}

	bool isEmpty() const
	{
		return top == -1;
	}  // end isEmpty

	bool push(const T& newEntry)
	{
		if (top == STACK_SIZE - 1) return false;	//Stack is FULL

		top++;
		count++;
		items[top] = newEntry;
		return true;
	}  // end push

	bool pop(T& TopEntry)
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		top--;
		count--;
		return true;
	}  // end pop

	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		return true;
	}  // end peek

	int get_count() const { return count; }

	   //Destructor
	~ArrayStack()
	{
		delete[]items;
	}

	//Copy constructor
	ArrayStack(const ArrayStack<T> & S) :STACK_SIZE(S.STACK_SIZE)
	{
		items = new T[STACK_SIZE];
		for (int i = 0; i <= S.top; i++)
			items[i] = S.items[i];
		top = S.top;
	}



}; // end ArrayStack



#include "ArrayStack.h"

#include <iostream>
using namespace std;

// A function to print stack contents
//as the function shoud operate on a class template, the function itself should be a function template as well
template <typename T>
void PrintStack(ArrayStack<T> S)
{

	cout << endl << "Printing stack contents: ";

	T x;
	while (S.pop(x))
		cout << x << " ";

	cout << endl;
}

//A function that takes a ArrayStack<T> S and returns its reverse (without affecting the orignal stack)
template <typename T>
ArrayStack<T> Reverse(ArrayStack<T> S)
{
	ArrayStack<T> Rev(S.getCapacity());	//First creat a stack 
	T Item;
	while (S.pop(Item))	//pop from S
		Rev.push(Item); //push into Rev

	return Rev;
}

//A function that takes a ArrayStack<T> S and replaces all occurrances of "value" with "newValue" (if found)
//if v is not found, it returns false
template <typename T>
bool ReplaceAll(ArrayStack<T> & Source, T value, T newValue)	//Source is passe by ref
{
	ArrayStack<T> tempS(Source.getCapacity());	//a temp stack is needed
	T x;
	bool found = false;
	while (Source.pop(x))
	{
		if (x == value)		//repalce the value if found
		{
			tempS.push(newValue);
			found = true;
		}
		else
			tempS.push(x);
	}

	//After the above loop, stack "Source" is empty
	//Restore Source stack again
	while (tempS.pop(x))
		Source.push(x);

	return found;
}
template <typename T>
void print(ArrayStack<T>& S)
{
	T Item;
	while (S.pop(Item)) cout << Item << " ";
}





#endif
