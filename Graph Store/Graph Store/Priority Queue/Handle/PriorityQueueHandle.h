#ifndef __PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__
#define __PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__

class PriorityQueueHandle
{
	template <class Item, class Key, class HandleUpdator, class KeyAccessor>
	friend class PriorityQueue;

public:
	PriorityQueueHandle() :
		PriorityQueueHandle(-1)
	{
	}

private:
	PriorityQueueHandle(long index) :
		index(index)
	{
	}

	bool isValid() const
	{
		return index != -1;
	}

private:
	long index;
};

#endif //__PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__