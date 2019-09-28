#include <assert.h>
#include <utility>

template <class Item, class Key, class KeyAccessor, class Function>
Hash<Item, Key, KeyAccessor, Function>::Table::Table(std::size_t size) :
	slots(size, size),
	count(0)
{
	for (auto i = 0u; i < size; ++i)
	{
		slots[i] = nullptr;
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
Hash<Item, Key, KeyAccessor, Function>::Table::Table(Table&& source) noexcept :
	slots(std::move(source.slots)),
	count(source.count)
{
	source.count = 0;
}

template <class Item, class Key, class KeyAccessor, class Function>
auto Hash<Item, Key, KeyAccessor, Function>::Table::operator=(Table&& rhs)
noexcept -> Table&
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline void
Hash<Item, Key, KeyAccessor, Function>::Table::swapContentsWith(Table t)
noexcept
{
	std::swap(slots, t.slots);
	std::swap(count, t.count);
}

template <class Item, class Key, class KeyAccessor, class Function>
inline void
Hash<Item, Key, KeyAccessor, Function>::Table::becomeEmptyWithSize(std::size_t size)
{
	*this = Table(size);
}

template <class Item, class Key, class KeyAccessor, class Function>
inline void
Hash<Item, Key, KeyAccessor, Function>::Table::addAt(std::size_t index,
                                                     Item& item) noexcept
{
	assert(!isOccupiedAt(index));

	slots[index] = std::addressof(item);
	++count;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool
Hash<Item, Key, KeyAccessor, Function>::Table::isOccupiedAt(std::size_t index)
const noexcept
{
	assert(index < slots.getSize());

	return slots[index] != nullptr;
}

template <class Item, class Key, class KeyAccessor, class Function>
Item* Hash<Item, Key, KeyAccessor, Function>::Table::extractItemAt(std::size_t index)
noexcept
{
	assert(isOccupiedAt(index));

	auto item = slots[index];
	slots[index] = nullptr;

	--count;

	return item;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline Item&
Hash<Item, Key, KeyAccessor, Function>::Table::operator[](std::size_t index)
noexcept
{
	const auto& table = *this;

	return const_cast<Item&>(table[index]);
}

template <class Item, class Key, class KeyAccessor, class Function>
inline const Item&
Hash<Item, Key, KeyAccessor, Function>::Table::operator[](std::size_t index)
const noexcept
{
	assert(isOccupiedAt(index));

	return *slots[index];
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t
Hash<Item, Key, KeyAccessor, Function>::Table::occupiedSlotsCount()
const noexcept
{
	return count;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t Hash<Item, Key, KeyAccessor, Function>::Table::size()
const noexcept
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
inline std::size_t
Hash<Item, Key, KeyAccessor, Function>::calculateTableSize(std::size_t expectedItemsCount)
noexcept
{
	return ((3 * expectedItemsCount) / 2) + MIN_TABLE_SIZE;
}

template <class Item, class Key, class KeyAccessor, class Function>
Hash<Item, Key, KeyAccessor, Function>::Hash(Hash&& source) :
    table(MIN_TABLE_SIZE),
    keyAccessor(std::move(source.keyAccessor)),
    hashFunction(std::move(source.hashFunction))
{
	std::swap(table, source.table);
}

template <class Item, class Key, class KeyAccessor, class Function>
Hash<Item, Key, KeyAccessor, Function>&
Hash<Item, Key, KeyAccessor, Function>::operator=(Hash&& rhs)
{
	if (this != &rhs)
	{
		auto hash = std::move(rhs);
		swap(hash);
	}

	return *this;
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::swap(Hash& hash) noexcept
{
    using std::swap;

    swap(table, hash.table);
    swap(keyAccessor, hash.keyAccessor);
    swap(hashFunction, hash.hashFunction);
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

	auto index = hashValueFor(keyAccessor(item));

	while (table.isOccupiedAt(index))
	{
		index = nextPositionToProbe(index);
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
inline bool Hash<Item, Key, KeyAccessor, Function>::isFillingUp()
const noexcept
{
	return 3 * table.occupiedSlotsCount() >= 2 * table.size();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t
Hash<Item, Key, KeyAccessor, Function>::hashValueFor(const Key& key)
const noexcept
{
	return hashFunction(key) % table.size();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t
Hash<Item, Key, KeyAccessor, Function>::nextPositionToProbe(std::size_t currentPosition)
const noexcept
{
	return (currentPosition + 1) % table.size();
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::rehashItemsInTableWithSize(std::size_t size)
{
	assert(size >= MIN_TABLE_SIZE);
	assert(size > table.occupiedSlotsCount());

	auto oldTable = std::move(table);

	try
	{
		table.becomeEmptyWithSize(size);
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
	auto size = table.size();

	for (auto i = 0u; i < size; ++i)
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
	const auto& hash = *this;

	return const_cast<Item&>(hash[key]);
}

template <class Item, class Key, class KeyAccessor, class Function>
const Item& Hash<Item, Key, KeyAccessor, Function>::operator[](const Key& key) const
{
	auto index = indexOfFirstItemWithKey(key);

	if (index != -1)
	{
		return table[index];
	}
	else
	{
        throw std::out_of_range{ "There is no item with such key!" };
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
long Hash<Item, Key, KeyAccessor, Function>::indexOfFirstItemWithKey(const Key& key) const
{
	auto index = hashValueFor(key);

	while (table.isOccupiedAt(index) &&
           keyAccessor(table[index]) != key)
	{
		index = nextPositionToProbe(index);
	}

	return table.isOccupiedAt(index) ? index : -1;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::contains(const Key& key) const
{
	return indexOfFirstItemWithKey(key) != -1;
}

template <class Item, class Key, class KeyAccessor, class Function>
Item* Hash<Item, Key, KeyAccessor, Function>::remove(const Key& key)
{
	auto removedItem = static_cast<Item*>(nullptr);
	auto index = indexOfFirstItemWithKey(key);

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
			rehashClusterStartingAt(nextPositionToProbe(index));
		}
	}

	return removedItem;
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::hasTooManyEmptySlots()
const noexcept
{
	return 6 * table.occupiedSlotsCount() <= table.size();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::canBeShrinked()
const noexcept
{
	return (table.size() / GROWTH_RATE) >= MIN_TABLE_SIZE;
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::shrinkAfterRemovingItemAt(std::size_t index)
{
	assert(!table.isOccupiedAt(index));
    assert(canBeShrinked());

	try
	{
		rehashItemsInTableWithSize(table.size() / GROWTH_RATE);
	}
	catch (std::bad_alloc&)
	{
		rehashClusterStartingAt(nextPositionToProbe(index));
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
void Hash<Item, Key, KeyAccessor, Function>::rehashClusterStartingAt(std::size_t index)
{
	while (table.isOccupiedAt(index))
	{
		add(*(table.extractItemAt(index)));
		index = nextPositionToProbe(index);
	}
}

template <class Item, class Key, class KeyAccessor, class Function>
inline std::size_t Hash<Item, Key, KeyAccessor, Function>::getCount() const noexcept
{
	return table.occupiedSlotsCount();
}

template <class Item, class Key, class KeyAccessor, class Function>
inline bool Hash<Item, Key, KeyAccessor, Function>::isEmpty() const noexcept
{
	return getCount() == 0;
}
