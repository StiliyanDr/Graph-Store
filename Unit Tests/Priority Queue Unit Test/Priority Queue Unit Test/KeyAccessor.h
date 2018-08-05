#pragma once

#include "Item.h"

class KeyAccessor
{
public:
	unsigned getKeyOf(const Item& item) const
	{
		return item.key;
	}

	void setKeyOfWith(Item& item, unsigned newKey) const
	{
		item.key = newKey;
	}
};