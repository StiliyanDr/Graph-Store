#include <cassert>
#include <utility>
#include <stdexcept>

template <class Item, class Key, class KeyAccessor>
inline Hash<Item, Key, KeyAccessor>::Hash(size_t expectedItemsCount)
{
	makeTableEmptyWithSize(calculateTableSize(expectedItemsCount));
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

template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::makeTableEmptyWithSize(size_t size)
{
	setTable(createEmptyTableWithSize(size));
	count = 0;
}

template <class Item, class Key, class KeyAccessor>
typename Hash<Item, Key, KeyAccessor>::Table Hash<Item, Key, KeyAccessor>::createEmptyTableWithSize(size_t size)
{
	assert(size >= MIN_TABLE_SIZE);

	Table table(size, size);
	table = emptyAllSlotsIn(std::move(table));

	return table;
}

template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::setTable(Table table)
{
	this->table = std::move(table);
	this->tableSize = this->table.getCount();

	assert(tableSize >= MIN_TABLE_SIZE);
}

template <class Item, class Key, class KeyAccessor>
typename Hash<Item, Key, KeyAccessor>::Table Hash<Item, Key, KeyAccessor>::emptyAllSlotsIn(Table table)
{
	size_t size = table.getCount();

	for (size_t i = 0; i < size; ++i)
	{
		table[i] = nullptr;
	}

	return table;
}

template <class Item, class Key, class KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(Hash<Item, Key, KeyAccessor>&& source)
{
	makeTableEmptyWithSize(MIN_TABLE_SIZE);

	std::swap(table, source.table);
	std::swap(count, source.count);
	std::swap(tableSize, source.tableSize);
	keyAccessor = std::move(source.keyAccessor);
	hashFunction = std::move(source.hashFunction);
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
void Hash<Item, Key, KeyAccessor>::swapContentsWith(Hash<Item, Key, KeyAccessor> hash)
{
	std::swap(count, hash.count);
	std::swap(tableSize, hash.tableSize);
	std::swap(table, hash.table);
	std::swap(keyAccessor, hash.keyAccessor);
	std::swap(hashFunction, hash.hashFunction);
}

template <class Item, class Key, class KeyAccessor>
inline void Hash<Item, Key, KeyAccessor>::empty()
{
	makeTableEmptyWithSize(MIN_TABLE_SIZE);
}

template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::add(Item& item)
{
	assert(tableSize >= MIN_TABLE_SIZE && count < tableSize);

	if (isFillingUp())
	{
		resize(GROWTH_RATE * tableSize);
	}

	size_t index = computeIndexFromKey(keyAccessor(item));

	while (table[index])
	{
		index = getNextPositionToProbe(index);
	}

	table[index] = &item;
	++count;
}

template <class Item, class Key, class KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isFillingUp() const
{
	return 3 * count >= 2 * tableSize;
}

template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::resize(size_t newSize)
{
	assert(newSize >= MIN_TABLE_SIZE && newSize > count);

	Table oldTable = std::move(table);

	try
	{
		makeTableEmptyWithSize(newSize);
	}
	catch (std::bad_alloc&)
	{
		table = std::move(oldTable);
		throw;
	}

	addAllItemsFrom(oldTable);
}

template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::addAllItemsFrom(Table& table)
{
	size_t size = table.getCount();

	for (size_t i = 0; i < size; ++i)
	{
		if (table[i])
		{
			add(*table[i]);
		}
	}
}

template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::remove(const Key& key)
{
	Item* removedItem = nullptr;
	long index = searchAndGetIndex(key);

	if (index != SEARCH_MISS)
	{
		assert(count > 0 && tableSize > count);
		removedItem = emptySlotAndReturnItemAt(index);

		if (hasTooManyEmptySlots() && tableCanBeShrinked())
		{
			resize(tableSize / GROWTH_RATE);
		}
		else
		{
			rehashCluster(getNextPositionToProbe(index));
		}
	}

	return removedItem;
}

template <class Item, class Key, class KeyAccessor>
long Hash<Item, Key, KeyAccessor>::searchAndGetIndex(const Key& key)
{
	if (!isEmpty())
	{
		assert(tableSize >= MIN_TABLE_SIZE);
		size_t index = computeIndexFromKey(key);

		while (table[index] && keyAccessor(*table[index]) != key)
		{
			index = getNextPositionToProbe(index);
		}

		if (table[index])
		{
			return index;
		}
	}

	return SEARCH_MISS;
}

template <class Item, class Key, class KeyAccessor>
inline size_t Hash<Item, Key, KeyAccessor>::computeIndexFromKey(const Key& key)
{
	return hashFunction(key) % tableSize;
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
		index = getNextPositionToProbe(index);
	}
}

template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::emptySlotAndReturnItemAt(size_t index)
{
	assert(table[index]);

	Item* removedItem = table[index];
	table[index] = nullptr;

	--count;

	return removedItem;
}

template <class Item, class Key, class KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::hasTooManyEmptySlots() const
{
	return 6 * count <= tableSize;
}

template <class Item, class Key, class KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::tableCanBeShrinked() const
{
	return (tableSize / GROWTH_RATE) >= MIN_TABLE_SIZE;
}

template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::search(const Key& key)
{
	long index = searchAndGetIndex(key);

	return (index != SEARCH_MISS) ? table[index] : nullptr;
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
inline size_t Hash<Item, Key, KeyAccessor>::getNextPositionToProbe(size_t currentPosition) const
{
	return (currentPosition + 1) % tableSize;
}
