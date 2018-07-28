#pragma once

#include "../../Graph Store/Graph Store/PriorityQueueHandle.h"

struct Item
{
	Item(unsigned key = 0) :
		key(key) { }

	unsigned key;
	PriorityQueueHandle handle;
};
