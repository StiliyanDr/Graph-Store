#include <assert.h>
#include <utility>

template <class Item, class Key, class Function, class KeyAccessor>
Hash<Item, Key, Function, KeyAccessor>::table_t::table_t(size_t size) :
	slots(size, size),
	count(0)
{
	for (size_t i = 0; i < size; ++i)
	{
		slots[i] = nullptr;
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
Hash<Item, Key, Function, KeyAccessor>::table_t::table_t(table_t&& source) :
	slots(std::move(source.slots)),
	count(source.count)
{
	source.count = 0;
}

template <class Item, class Key, class Function, class KeyAccessor>
typename Hash<Item, Key, Function, KeyAccessor>::table_t&
Hash<Item, Key, Function, KeyAccessor>::table_t::operator=(table_t&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline void Hash<Item, Key, Function, KeyAccessor>::table_t::swapContentsWith(table_t table)
{
	std::swap(slots, table.slots);
	std::swap(count, table.count);
}

template <class Item, class Key, class Function, class KeyAccessor>
inline void Hash<Item, Key, Function, KeyAccessor>::table_t::becomeEmptyWithSize(size_t size)
{
	*this = table_t(size);
}

template <class Item, class Key, class Function, class KeyAccessor>
inline void Hash<Item, Key, Function, KeyAccessor>::table_t::addAt(size_t index, Item& item)
{
	assert(!isOccupiedAt(index));

	slots[index] = &item;
	++count;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::table_t::isOccupiedAt(size_t index) const
{
	assert(index < slots.getSize());

	return slots[index] != nullptr;
}

template <class Item, class Key, class Function, class KeyAccessor>
Item* Hash<Item, Key, Function, KeyAccessor>::table_t::extractItemAt(size_t index)
{
	assert(isOccupiedAt(index));

	Item* item = slots[index];
	slots[index] = nullptr;

	--count;

	return item;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline Item& Hash<Item, Key, Function, KeyAccessor>::table_t::operator[](size_t index)
{
	assert(isOccupiedAt(index));

	return *slots[index];
}

template <class Item, class Key, class Function, class KeyAccessor>
inline const Item& Hash<Item, Key, Function, KeyAccessor>::table_t::operator[](size_t index) const
{
	assert(isOccupiedAt(index));

	return *slots[index];
}

template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::table_t::occupiedSlotsCount() const
{
	return count;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::table_t::size() const
{
	return slots.getSize();
}

template <class Item, class Key, class Function, class KeyAccessor>
inline Hash<Item, Key, Function, KeyAccessor>::Hash(size_t expectedItemsCount)
{
	table.becomeEmptyWithSize(calculateTableSize(expectedItemsCount));
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
	return ((3 * expectedItemsCount) / 2) + MIN_TABLE_SIZE;
}

template <class Item, class Key, class Function, class KeyAccessor>
Hash<Item, Key, Function, KeyAccessor>::Hash(Hash<Item, Key, Function, KeyAccessor>&& source)
{
	table.becomeEmptyWithSize(MIN_TABLE_SIZE);

	std::swap(table, source.table);
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
	std::swap(table, hash.table);
	std::swap(keyAccessor, hash.keyAccessor);
	std::swap(hashFunction, hash.hashFunction);
}

template <class Item, class Key, class Function, class KeyAccessor>
inline void Hash<Item, Key, Function, KeyAccessor>::empty()
{
	table.becomeEmptyWithSize(MIN_TABLE_SIZE);
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::add(Item& item)
{
	assert(table.size() >= MIN_TABLE_SIZE);
	assert(table.occupiedSlotsCount() < table.size());

	extendIfFillingUp();

	size_t index = computeIndexFromKey(keyAccessor(item));

	while (table.isOccupiedAt(index))
	{
		index = getNextPositionToProbe(index);
	}

	table.addAt(index, item);
}

template <class Item, class Key, class Function, class KeyAccessor>
inline void Hash<Item, Key, Function, KeyAccessor>::extendIfFillingUp()
{
	if (isFillingUp())
	{
		resize(GROWTH_RATE * table.size());
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::isFillingUp() const
{
	return 3 * table.occupiedSlotsCount() >= 2 * table.size();
}

template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::computeIndexFromKey(const Key& key) const
{
	return hashFunction(key) % table.size();
}

template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::getNextPositionToProbe(size_t currentPosition) const
{
	return (currentPosition + 1) % table.size();
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::resize(size_t newSize)
{
	assert(newSize >= MIN_TABLE_SIZE);
	assert(newSize > table.occupiedSlotsCount());

	Table oldTable = std::move(table);

	try
	{
		table.becomeEmptyWithSize(newSize);
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
	size_t size = table.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (table.isOccupiedAt(i))
		{
			add(table[i]);
		}
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
Item& Hash<Item, Key, Function, KeyAccessor>::operator[](const Key& key)
{
	long index = getIndexOfFirstItemWithKey(key);

	if (index != -1)
	{
		return table[index];
	}
	else
	{
		throw std::logic_error("There is no item with such key!");
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
const Item& Hash<Item, Key, Function, KeyAccessor>::operator[](const Key& key) const
{
	long index = getIndexOfFirstItemWithKey(key);

	if (index != -1)
	{
		return table[index];
	}
	else
	{
		throw std::logic_error("There is no item with such key!");
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
long Hash<Item, Key, Function, KeyAccessor>::getIndexOfFirstItemWithKey(const Key& key) const
{
	size_t index = computeIndexFromKey(key);

	while (table.isOccupiedAt(index) && keyAccessor(table[index]) != key)
	{
		index = getNextPositionToProbe(index);
	}

	return table.isOccupiedAt(index) ? index : -1;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::contains(const Key& key) const
{
	return getIndexOfFirstItemWithKey(key) != -1;
}

template <class Item, class Key, class Function, class KeyAccessor>
Item* Hash<Item, Key, Function, KeyAccessor>::remove(const Key& key)
{
	Item* removedItem = nullptr;
	long index = getIndexOfFirstItemWithKey(key);

	if (index != -1)
	{
		assert(table.occupiedSlotsCount() > 0);
		assert(table.size() > table.occupiedSlotsCount());

		removedItem = table.extractItemAt(index);

		if (hasTooManyEmptySlots() && tableCanBeShrinked())
		{
			shrinkAfterRemovingItemAt(index);
		}
		else
		{
			rehashClusterStartingAt(getNextPositionToProbe(index));
		}
	}

	return removedItem;
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::hasTooManyEmptySlots() const
{
	return 6 * table.occupiedSlotsCount() <= table.size();
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::tableCanBeShrinked() const
{
	return (table.size() / GROWTH_RATE) >= MIN_TABLE_SIZE;
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::shrinkAfterRemovingItemAt(size_t index)
{
	assert(!table.isOccupiedAt(index));

	try
	{
		resize(table.size() / GROWTH_RATE);
	}
	catch (std::bad_alloc&)
	{
		rehashClusterStartingAt(getNextPositionToProbe(index));
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
void Hash<Item, Key, Function, KeyAccessor>::rehashClusterStartingAt(size_t index)
{
	Item* itemToRehash;

	while (table.isOccupiedAt(index))
	{
		itemToRehash = table.extractItemAt(index);
		add(*itemToRehash);
		index = getNextPositionToProbe(index);
	}
}

template <class Item, class Key, class Function, class KeyAccessor>
inline size_t Hash<Item, Key, Function, KeyAccessor>::getCount() const
{
	return table.occupiedSlotsCount();
}

template <class Item, class Key, class Function, class KeyAccessor>
inline bool Hash<Item, Key, Function, KeyAccessor>::isEmpty() const
{
	return getCount() == 0;
}
