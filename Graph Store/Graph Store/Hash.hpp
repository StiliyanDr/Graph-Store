#include <cassert>
#include <utility>
#include <stdexcept>


template <class Item, class Key, class KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(size_t expectedCount) :
	count(0),
	tableSize(calculateTableSize(expectedCount)),
	table(tableSize, tableSize)
{
	nullify(table);
}


template <class Item, class Key, class KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(Hash<Item, Key, KeyAccessor>&& source) :
	count(0),
	tableSize(MIN_TABLE_SIZE),
	table(tableSize, tableSize),
	keyAccessor(std::move(source.keyAccessor)),
	hashFunction(std::move(source.hashFunction))
{
	nullify(table);

	std::swap(table, source.table);
	std::swap(count, source.count);
	std::swap(tableSize, source.tableSize);
}


template <class Item, class Key, class KeyAccessor>
Hash<Item, Key, KeyAccessor>& Hash<Item, Key, KeyAccessor>::operator=(Hash<Item, Key, KeyAccessor>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}
	return *this;
}


template <class Item, class Key, class KeyAccessor>
Hash<Item, Key, KeyAccessor>& Hash<Item, Key, KeyAccessor>::operator=(const Hash<Item, Key, KeyAccessor>& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(rhs);
	}
	return *this;
}


template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::search(const Key& key)
{
	long index = searchAndGetIndex(key);

	return (index != SEARCH_MISS) ? table[index] : nullptr;
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::add(Item& item)
{
	assert(tableSize >= MIN_TABLE_SIZE && count < tableSize);

	if (isFillingUp())
	{
		resize(2 * tableSize);
	}

	size_t index = hashFunction(keyAccessor(item)) % tableSize;

	while (table[index])
	{
		index = (index + 1) % tableSize;
	}

	table[index] = &item;
	++count;
}


template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::remove(const Key& key)
{
	Item* removed = nullptr;
	long index = searchAndGetIndex(key);

	if (index != SEARCH_MISS)
	{
		assert(count > 0 && tableSize > count);
		removed = emptySlotAndReturnItemAt(index);

		if (hasTooManyEmptySlots() && tableCanBeHalved())
		{
			resize(tableSize / 2);
		}
		else
		{
			rehashCluster((index + 1) % tableSize);
		}
	}

	return removed;
}


template <class Item, class Key, class KeyAccessor>
inline size_t Hash<Item, Key, KeyAccessor>::getCount() const
{
	return count;
}


template <class Item, class Key, class KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isEmpty() const
{
	return count == 0;
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::empty()
{
	table.empty();
	table.ensureSize(MIN_TABLE_SIZE);

	for (size_t i = 1; i <= MIN_TABLE_SIZE; ++i)
	{
		table.add(nullptr);
	}

	tableSize = MIN_TABLE_SIZE;
	count = 0;
}


template <class Item, class Key, class KeyAccessor>
long Hash<Item, Key, KeyAccessor>::searchAndGetIndex(const Key& key)
{
	if (!isEmpty())
	{
		assert(tableSize >= MIN_TABLE_SIZE);
		size_t index = hashFunction(key) % tableSize;

		while (table[index] && keyAccessor(*table[index]) != key)
		{
			index = (index + 1) % tableSize;
		}

		if (table[index])
		{
			return index;
		}
	}

	return SEARCH_MISS;
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::resize(size_t newSize)
{
	assert(newSize >= MIN_TABLE_SIZE && newSize > count);

	DynamicArray<Item*> buffer(newSize, newSize);
	nullify(buffer);

	std::swap(table, buffer);

	size_t oldTableSize = tableSize;
	tableSize = newSize;
	count = 0;

	for (size_t i = 0; i < oldTableSize; ++i)
	{
		if (buffer[i])
		{
			add(*buffer[i]);
		}
	}
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::nullify(DynamicArray<Item*>& table)
{
	size_t size = table.getCount();

	for (size_t i = 0; i < size; ++i)
	{
		table[i] = nullptr;
	}
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::rehashCluster(size_t start)
{
	assert(start < tableSize);
	
	size_t index = start;
	Item* itemToRehash;

	while (table[index])
	{
		itemToRehash = emptySlotAndReturnItemAt(index);
		add(*itemToRehash);
		index = (index + 1) % tableSize;
	}
}


template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::emptySlotAndReturnItemAt(size_t index)
{
	assert(table[index]);

	Item* removed = table[index];
	table[index] = nullptr;
	
	--count;
	
	return removed;
}


template <class Item, class Key, class KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::hasTooManyEmptySlots() const
{
	return 6 * count <= tableSize;
}


template <class Item, class Key, class KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::tableCanBeHalved() const
{
	return (tableSize / 2) >= MIN_TABLE_SIZE;
}


template <class Item, class Key, class KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isFillingUp() const
{
	return 3 * count >= 2 * tableSize;
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::swapContentsWith(Hash<Item, Key, KeyAccessor> temp)
{
	std::swap(count, temp.count);
	std::swap(tableSize, temp.tableSize);
	std::swap(table, temp.table);
	std::swap(keyAccessor, temp.keyAccessor);
	std::swap(hashFunction, temp.hashFunction);
}


///
/// With this expression, when expectedItemsCount items are inserted into the
/// table, the load factor will be 2 / 3. 
/// Adding MIN_TABLE_SIZE prevents from inappropriate table size for small values 
/// of expectedItemsCount.
///
template <class Item, class Key, class KeyAccessor>
size_t Hash<Item, Key, KeyAccessor>::calculateTableSize(size_t expectedItemsCount)
{
	if (expectedItemsCount > 0)
	{
		return ((3 * expectedItemsCount) / 2) + MIN_TABLE_SIZE;
	}
	else
	{
		throw std::invalid_argument("The expected items count must be positive!");
	}
}
