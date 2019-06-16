#ifndef __PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__
#define __PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__

#include <assert.h>

class PriorityQueueHandle
{
	template <class Item,
		class Comparator,
		class Key,
		class KeyAccessor,
		class HandleUpdator>
	friend class PriorityQueue;

public:
	PriorityQueueHandle() noexcept :
		PriorityQueueHandle(-1)
	{
	}

	bool isValid() const noexcept
	{
		return index != -1;
	}

private:
	explicit PriorityQueueHandle(long index) noexcept :
		index(index)
	{
		assert(index >= -1);
	}

private:
	long index;
};

#endif //__PRIORITY_QUEUE_HANDLE_HEADER_INCLUDED__