#ifndef __DYNAMIC_QUEUE_HEADER_INCLUDED__
#define __DYNAMIC_QUEUE_HEADER_INCLUDED__

#include "../../Linked List/Linked List/LinkedList.h"

template <class T>
class DynamicQueue
{
public:
	bool isEmpty() const;
	void enqueue(const T& item);
	T dequeue();
	T first() const;

private:
	LinkedList<T> list;
};

#include "DynamicQueue.hpp"

#endif //__DYNAMIC_QUEUE_HEADER_INCLUDED__