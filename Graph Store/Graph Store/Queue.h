#ifndef __QUEUE_HEADER_INCLUDED__
#define __QUEUE_HEADER_INCLUDED__

#include "LinkedList.h"

template <class T>
class Queue
{
public:
	void enqueue(const T& item);
	T dequeue();
	T first() const;

	bool isEmpty() const;
	void empty();

private:
	LinkedList<T> items;
};

#include "Queue.hpp"

#endif //__QUEUE_HEADER_INCLUDED__