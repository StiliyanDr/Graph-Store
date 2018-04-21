#ifndef __QUEUE_HEADER_INCLUDED__
#define __QUEUE_HEADER_INCLUDED__

#include "LinkedList.h"

template <class T>
class Queue
{
public:
	bool isEmpty() const;
	void enqueue(const T& item);
	T dequeue();
	T first() const;

private:
	LinkedList<T> items;
};

#include "Queue.hpp"

#endif //__QUEUE_HEADER_INCLUDED__