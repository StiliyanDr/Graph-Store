#include <assert.h>
#include <utility>

template <class Item, class Key, class Function, class KeyAccessor>
inline Hash<Item, Key, Function, KeyAccessor>::Hash(size_t expectedItemsCount)
{
	makeTableEmptyWithSize(calculateTableSize(expectedItemsCount));
}

///
/// With this expression, when expectedItemsCount items are inserted into the
/// table, the load factor will be 2 / 3. 
/// Adding MIN_TABLE_SIZE prevents from inappropriate table size for small values 
/// of expectedItemsCount.
///
template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::calculateTableSize(size_t expectedItemsCount)
{
	assert(expectedItemsCount > 0);

	return ((3 * expectedItemsCount) / 2) + MIN_TABLE_SIZE;
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::makeTableEmptyWithSize(size_t size)
{
	setTable(createEmptyTableWithSize(size));
	count = 0;
}

template <class Item, class Key, class Function, class KeyAccessor>
typename Hash<Item, Key, Function, KeyAccessor>::Table
Hash<Item, Key, Function, KeyAccessor>::createEmptyTableWithSize(size_t size)
{
	assert(size >= MIN_TABLE_SIZE);

	Table table(size, size);
	table = emptyAllSlotsIn(std::move(table));

	return table;
}

template <class Item, class Key, class Function, class KeyAccessor>
typename Hash<Item, Key, Function, KeyAccessor>::Table
Hash<Item, Key, Function, KeyAccessor>::emptyAllSlotsIn(Table table)
{
	size_t size = table.getSize();

	for (size_t i = 0; i < size; ++i)
	{
		table[i] = nullptr;
	}

	return table;
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::setTable(Table table)
{
	this->table = std::move(table);
	
	assert(this->table.getSize() >= MIN_TABLE_SIZE);
}

template <class Item, class Key, class Function, class KeyAccessor>
Hash<Item, Key, Function, KeyAccessor>::Hash(Hash<Item, Key, Function, KeyAccessor>&& source)
{
	makeTableEmptyWithSize(MIN_TABLE_SIZE);

	std::swap(table, source.table);
	std::swap(count, source.count);

	keyAccessor = std::move(source.keyAccessor);
	hashFunction = std::move(source.hashFunction);
}

template <class Item, class Key, class Function, class KeyAccessor>
Hash<Item, Key, Function, KeyAccessor>&
Hash<Item, Key, Function, KeyAccessor>::operator=(Hash<Item, Key, Function, KeyAccessor>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::swapContentsWith(Hash<Item, Key, Function, KeyAccessor> hash)
{
	std::swap(count, hash.count);
	std::swap(table, hash.table);
	std::swap(keyAccessor, hash.keyAccessor);
	std::swap(hashFunction, hash.hashFunction);
}

template <class Item, class Key, class Function, class KeyAccessor>
inline void Hash<Item, Key, Function, KeyAccessor>::empty()
{
	makeTableEmptyWithSize(MIN_TABLE_SIZE);
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::add(Item& item)
{
	assert(table.getSize() >= MIN_TABLE_SIZE);
	assert(count < table.getSize());

	extendIfFillingUp();

	size_t index = computeIndexFromKey(keyAccessor(item));

	while (table[index])
	{
		index = getNextPositionToProbe(index);
	}

	table[index] = &item;
	++count;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline void Hash<Item, Key, Function, KeyAccessor>::extendIfFillingUp()
{
	if (isFillingUp())
	{
		resize(GROWTH_RATE * table.getSize());
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::isFillingUp() const
{
	return 3 * count >= 2 * table.getSize();
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::resize(size_t newSize)
{
	assert(newSize >= MIN_TABLE_SIZE);
	assert(newSize > count);

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

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::addAllItemsFrom(Table& table)
{
	size_t size = table.getSize();

	for (size_t i = 0; i < size; ++i)
	{
		if (table[i])
		{
			add(*table[i]);
		}
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
Item* Hash<Item, Key, Function, KeyAccessor>::search(const Key& key)
{
	long index = getIndexOfFirstItemWithKey(key);

	return (index != -1) ? table[index] : nullptr;
}

template <class Item, class Key, class Function, class KeyAccessor>
long Hash<Item, Key, Function, KeyAccessor>::getIndexOfFirstItemWithKey(const Key& key)
{
	size_t index = computeIndexFromKey(key);

	while (table[index] && keyAccessor(*table[index]) != key)
	{
		index = getNextPositionToProbe(index);
	}

	return (table[index]) ? index : -1;
}

template <class Item, class Key, class Function, class KeyAccessor>
Item* Hash<Item, Key, Function, KeyAccessor>::remove(const Key& key)
{
	Item* removedItem = nullptr;
	long index = getIndexOfFirstItemWithKey(key);

	if (index != -1)
	{
		assert(count > 0 && table.getSize() > count);
		removedItem = emptySlotAndReturnItemAt(index);

		if (hasTooManyEmptySlots() && tableCanBeShrinked())
		{
			resize(table.getSize() / GROWTH_RATE);
		}
		else
		{
			rehashClusterStartingAt(getNextPositionToProbe(index));
		}
	}

	return removedItem;
}

template <class Item, class Key, class Function, class KeyAccessor>
Item* Hash<Item, Key, Function, KeyAccessor>::emptySlotAndReturnItemAt(size_t index)
{
	assert(index < table.getSize());
	assert(table[index]);

	Item* removedItem = table[index];
	table[index] = nullptr;

	--count;

	return removedItem;
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::rehashClusterStartingAt(size_t index)
{
	assert(index < table.getSize());

	Item* itemToRehash;

	while (table[index])
	{
		itemToRehash = emptySlotAndReturnItemAt(index);
		add(*itemToRehash);
		index = getNextPositionToProbe(index);
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::hasTooManyEmptySlots() const
{
	return 6 * count <= table.getSize();
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::tableCanBeShrinked() const
{
	return (table.getSize() / GROWTH_RATE) >= MIN_TABLE_SIZE;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::computeIndexFromKey(const Key& key) const
{
	return hashFunction(key) % table.getSize();
}

template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::getNextPositionToProbe(size_t currentPosition) const
{
	return (currentPosition + 1) % table.getSize();
}

template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::getCount() const
{
	return count;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::isEmpty() const
{
	return count == 0;
}
