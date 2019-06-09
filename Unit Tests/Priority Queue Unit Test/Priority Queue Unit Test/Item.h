#pragma once

#include "Priority Queue/PriorityQueueHandle.h"

struct Item
{
	Item(unsigned key = 0) :
		key(key) { }

	unsigned key;
	PriorityQueueHandle handle;
};
