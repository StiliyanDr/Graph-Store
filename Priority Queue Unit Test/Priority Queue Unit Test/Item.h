#pragma once

#include "../../Graph Store/Graph Store/PriorityQueueHandle.h"

struct Item
{
	Item(unsigned number = 0) :
		number(number) { }

	unsigned number;
	PriorityQueueHandle handle;
};
