#include "PriorityQueueHandle.h"

PriorityQueueHandle::PriorityQueueHandle() :
	PriorityQueueHandle(-1)
{
}

PriorityQueueHandle::PriorityQueueHandle(long index) :
	index(index)
{
}

bool PriorityQueueHandle::isValid() const
{
	return index != -1;
}
