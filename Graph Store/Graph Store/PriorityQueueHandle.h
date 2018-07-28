#ifndef __PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__
#define __PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__

class PriorityQueueHandle
{
	template <class Item, class Key, class HandleUpdator, class KeyAccessor>
	friend class PriorityQueue;

public:
	PriorityQueueHandle();

private:
	PriorityQueueHandle(long index);
	bool isValid() const;

private:
	long index;
};

#endif //__PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__