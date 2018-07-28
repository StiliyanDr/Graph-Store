#pragma once

#include "Item.h"

class KeyAccessor
{
public:
	unsigned getKeyOf(const Item& item) const
	{
		return item.number;
	}

	void setKeyOfWith(Item& item, unsigned newNumber) const
	{
		item.number = newNumber;
	}
};
