#ifndef __QUEUE_HEADER_INCLUDED__
#define __QUEUE_HEADER_INCLUDED__

#include "../Linked List/LinkedList.h"
#include <stdexcept>

template <class T>
class Queue
{
public:
	void enqueue(const T& item)
	{
		items.addBack(item);
	}
	
	T dequeue()
	{
		T firstItem(first());

		items.removeFirst();

		return firstItem;
	}

	T first() const
	{
		if (!isEmpty())
		{
			return items.getFirst();
		}
		else
		{
			throw std::logic_error("The queue is empty!");
		}
	}

	bool isEmpty() const
	{
		return items.isEmpty();
	}

	void empty()
	{
		items.empty();
	}

private:
	LinkedList<T> items;
};

#endif //__QUEUE_HEADER_INCLUDED__