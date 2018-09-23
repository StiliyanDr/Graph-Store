#pragma once

#include "Item.h"

class KeyAccessor
{
public:
	const unsigned& getKeyOf(Item* item) const
	{
		return item->key;
	}

	void setKeyOfWith(Item* item, unsigned newKey) const
	{
		item->key = newKey;
	}
};
