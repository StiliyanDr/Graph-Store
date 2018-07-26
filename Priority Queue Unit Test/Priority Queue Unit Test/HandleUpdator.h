#pragma once
#include "Item.h"

class HandleUpdator
{
public:
	void operator()(Item& item, const PriorityQueueHandle& handle) const
	{
		item.handle = handle;
	}
};
