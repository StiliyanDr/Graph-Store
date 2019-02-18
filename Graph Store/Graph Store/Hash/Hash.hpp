#include <assert.h>
#include <utility>

template <class Item, class Key, class KeyAccessor, class Function>
Hash<Item, Key, KeyAccessor, Function>::Table::Table(std::size_t size) :
	slots(size, size),
	count(0)
{
	for (std::size_t i = 0; i < size; ++i)
	{
		slots[i] = nullptr;
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
Hash<Item, Key, KeyAccessor, Function>::Table::Table(Table&& source) :
	slots(std::move(source.slots)),
	count(source.count)
{
	source.count = 0;
}

template <class Item, class Key, class KeyAccessor, class Function>
typename Hash<Item, Key, KeyAccessor, Function>::Table&
Hash<Item, Key, KeyAccessor, Function>::Table::operator=(Table&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline void Hash<Item, Key, KeyAccessor, Function>::Table::swapContentsWith(Table table)
{
	std::swap(slots, table.slots);
	std::swap(count, table.count);
}

template <class Item, class Key, class KeyAccessor, class Function>
inline void Hash<Item, Key, KeyAccessor, Function>::Table::becomeEmptyWithSize(std::size_t size)
{
	*this = Table(size);
}

template <class Item, class Key, class KeyAccessor, class Function>
inline void Hash<Item, Key, KeyAccessor, Function>::Table::addAt(std::size_t index, Item& item)
{
	assert(!isOccupiedAt(index));

	slots[index] = &item;
	++count;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::Table::isOccupiedAt(std::size_t index) const
{
	assert(index < slots.getSize());

	return slots[index] != nullptr;
}

template <class Item, class Key, class KeyAccessor, class Function>
Item* Hash<Item, Key, KeyAccessor, Function>::Table::extractItemAt(std::size_t index)
{
	assert(isOccupiedAt(index));

	Item* item = slots[index];
	slots[index] = nullptr;

	--count;

	return item;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline Item& Hash<Item, Key, KeyAccessor, Function>::Table::operator[](std::size_t index)
{
	assert(isOccupiedAt(index));

	return *slots[index];
}

template <class Item, class Key, class KeyAccessor, class Function>
inline const Item& Hash<Item, Key, KeyAccessor, Function>::Table::operator[](std::size_t index) const
{
	assert(isOccupiedAt(index));

	return *slots[index];
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t Hash<Item, Key, KeyAccessor, Function>::Table::occupiedSlotsCount() const
{
	return count;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t Hash<Item, Key, KeyAccessor, Function>::Table::size() const
{
	return slots.getSize();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline Hash<Item, Key, KeyAccessor, Function>::Hash(std::size_t expectedItemsCount)
{
	table.becomeEmptyWithSize(calculateTableSize(expectedItemsCount));
}

///
/// With this expression, when expectedItemsCount items are inserted into the
/// table, the load factor will be 2 / 3. 
/// Adding MIN_TABLE_SIZE prevents from inappropriate table size for small values 
/// of expectedItemsCount.
///
template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t Hash<Item, Key, KeyAccessor, Function>::calculateTableSize(std::size_t expectedItemsCount)
{
	return ((3 * expectedItemsCount) / 2) + MIN_TABLE_SIZE;
}

template <class Item, class Key, class KeyAccessor, class Function>
Hash<Item, Key, KeyAccessor, Function>::Hash(Hash<Item, Key, KeyAccessor, Function>&& source)
{
	table.becomeEmptyWithSize(MIN_TABLE_SIZE);

	std::swap(table, source.table);
	keyAccessor = std::move(source.keyAccessor);
	hashFunction = std::move(source.hashFunction);
}

template <class Item, class Key, class KeyAccessor, class Function>
Hash<Item, Key, KeyAccessor, Function>&
Hash<Item, Key, KeyAccessor, Function>::operator=(Hash<Item, Key, KeyAccessor, Function>&& rhs)
{
	if (this != &rhs)
	{
		Hash<Item, Key, KeyAccessor, Function> hash(std::move(rhs));
		swap(hash);
	}

	return *this;
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::swap(Hash<Item, Key, KeyAccessor, Function>& hash) noexcept
{
	std::swap(table, hash.table);
	std::swap(keyAccessor, hash.keyAccessor);
	std::swap(hashFunction, hash.hashFunction);
}

template <class Item, class Key, class KeyAccessor, class Function>
inline void Hash<Item, Key, KeyAccessor, Function>::empty()
{
	table.becomeEmptyWithSize(MIN_TABLE_SIZE);
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::add(Item& item)
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

template <class Item, class Key, class KeyAccessor, class Function>
inline void Hash<Item, Key, KeyAccessor, Function>::extendIfFillingUp()
{
	if (isFillingUp())
	{
		rehashItemsInTableWithSize(GROWTH_RATE * table.size());
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::isFillingUp() const
{
	return 3 * table.occupiedSlotsCount() >= 2 * table.size();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t Hash<Item, Key, KeyAccessor, Function>::computeIndexFromKey(const Key& key) const
{
	return hashFunction(key) % table.size();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t Hash<Item, Key, KeyAccessor, Function>::getNextPositionToProbe(std::size_t currentPosition) const
{
	return (currentPosition + 1) % table.size();
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::rehashItemsInTableWithSize(std::size_t newSize)
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

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::addAllItemsFrom(Table& table)
{
	std::size_t size = table.size();

	for (std::size_t i = 0; i < size; ++i)
	{
		if (table.isOccupiedAt(i))
		{
			add(table[i]);
		}
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
Item& Hash<Item, Key, KeyAccessor, Function>::operator[](const Key& key)
{
	const Hash<Item, Key, KeyAccessor, Function>& hash = *this;

	return const_cast<Item&>(hash[key]);
}

template <class Item, class Key, class KeyAccessor, class Function>
const Item& Hash<Item, Key, KeyAccessor, Function>::operator[](const Key& key) const
{
	long index = getIndexOfFirstItemWithKey(key);

	if (index != -1)
	{
		return table[index];
	}
	else
	{
		throw std::out_of_range("There is no item with such key!");
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
long Hash<Item, Key, KeyAccessor, Function>::getIndexOfFirstItemWithKey(const Key& key) const
{
	std::size_t index = computeIndexFromKey(key);

	while (table.isOccupiedAt(index) && keyAccessor(table[index]) != key)
	{
		index = getNextPositionToProbe(index);
	}

	return table.isOccupiedAt(index) ? index : -1;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::contains(const Key& key) const
{
	return getIndexOfFirstItemWithKey(key) != -1;
}

template <class Item, class Key, class KeyAccessor, class Function>
Item* Hash<Item, Key, KeyAccessor, Function>::remove(const Key& key)
{
	Item* removedItem = nullptr;
	long index = getIndexOfFirstItemWithKey(key);

	if (index != -1)
	{
		assert(table.occupiedSlotsCount() > 0);
		assert(table.size() > table.occupiedSlotsCount());

		removedItem = table.extractItemAt(index);

		if (hasTooManyEmptySlots() && canBeShrinked())
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

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::hasTooManyEmptySlots() const
{
	return 6 * table.occupiedSlotsCount() <= table.size();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::canBeShrinked() const
{
	return (table.size() / GROWTH_RATE) >= MIN_TABLE_SIZE;
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::shrinkAfterRemovingItemAt(std::size_t index)
{
	assert(!table.isOccupiedAt(index));

	try
	{
		rehashItemsInTableWithSize(table.size() / GROWTH_RATE);
	}
	catch (std::bad_alloc&)
	{
		rehashClusterStartingAt(getNextPositionToProbe(index));
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::rehashClusterStartingAt(std::size_t index)
{
	Item* itemToRehash;

	while (table.isOccupiedAt(index))
	{
		itemToRehash = table.extractItemAt(index);
		add(*itemToRehash);
		index = getNextPositionToProbe(index);
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t Hash<Item, Key, KeyAccessor, Function>::getCount() const
{
	return table.occupiedSlotsCount();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::isEmpty() const
{
	return getCount() == 0;
}
