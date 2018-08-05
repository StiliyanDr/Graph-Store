#pragma once

#include "../../../Graph Store/Graph Store/Priority Queue/Handle/PriorityQueueHandle.h"

struct Item
{
	Item(unsigned key = 0) :
		key(key) { }

	unsigned key;
	PriorityQueueHandle handle;
};
