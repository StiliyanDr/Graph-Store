#include <assert.h>

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::Table(std::size_t size) :
	slots(size, size),
	count(0)
{
	for (auto i = 0u; i < size; ++i)
	{
		slots[i] = nullptr;
	}
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::Table(Table&& source) noexcept :
	slots(std::move(source.slots)),
	count(source.count)
{
	source.count = 0;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
auto Hash<Item, Key, KeyAccessor, Function, Equal>::Table::operator=(Table&& rhs)
noexcept -> Table&
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline void
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::swapContentsWith(Table t)
noexcept
{
	std::swap(slots, t.slots);
	std::swap(count, t.count);
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline void
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::becomeEmptyWithSize(std::size_t size)
{
	*this = Table(size);
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline void
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::addAt(std::size_t index,
                                                            Item& item) noexcept
{
	assert(!isOccupiedAt(index));

	slots[index] = std::addressof(item);
	++count;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline bool
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::isOccupiedAt(std::size_t index)
const noexcept
{
	assert(index < slots.getSize());

	return slots[index] != nullptr;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
Item* Hash<Item, Key, KeyAccessor, Function, Equal>::Table::extractItemAt(std::size_t index)
noexcept
{
	assert(isOccupiedAt(index));

	auto item = slots[index];
	slots[index] = nullptr;

	--count;

	return item;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline Item&
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::operator[](std::size_t index)
noexcept
{
	const auto& table = *this;

	return const_cast<Item&>(table[index]);
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline const Item&
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::operator[](std::size_t index)
const noexcept
{
	assert(isOccupiedAt(index));

	return *slots[index];
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline std::size_t
Hash<Item, Key, KeyAccessor, Function, Equal>::Table::occupiedSlotsCount()
const noexcept
{
	return count;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline std::size_t Hash<Item, Key, KeyAccessor, Function, Equal>::Table::size()
const noexcept
{
	return slots.getSize();
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
Hash<Item, Key, KeyAccessor, Function, Equal>::Hash(
    std::size_t expectedItemsCount,
    KeyAccessor accessor,
    Function hashFunction,
    Equal areEqual
) :
    table(calculateTableSize(expectedItemsCount)),
    keyAccessor(accessor),
    hashFunction(hashFunction),
    areEqual(areEqual)
{
}

///
/// With this expression, when expectedItemsCount items are inserted into the
/// table, the load factor will be 2 / 3. 
/// Adding MIN_TABLE_SIZE prevents from inappropriate table size for small values 
/// of expectedItemsCount.
///
template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline std::size_t
Hash<Item, Key, KeyAccessor, Function, Equal>::calculateTableSize(std::size_t expectedItemsCount)
noexcept
{
	return ((3 * expectedItemsCount) / 2) + MIN_TABLE_SIZE;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
Hash<Item, Key, KeyAccessor, Function, Equal>::Hash(Hash&& source) :
    table(MIN_TABLE_SIZE),
    keyAccessor(std::move(source.keyAccessor)),
    hashFunction(std::move(source.hashFunction)),
    areEqual(std::move(source.areEqual))
{
	std::swap(table, source.table);
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
Hash<Item, Key, KeyAccessor, Function, Equal>&
Hash<Item, Key, KeyAccessor, Function, Equal>::operator=(Hash&& rhs)
{
	if (this != &rhs)
	{
		auto hash = std::move(rhs);
		swap(hash);
	}

	return *this;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
void Hash<Item, Key, KeyAccessor, Function, Equal>::swap(Hash& hash) noexcept
{
    using std::swap;

    swap(table, hash.table);
    swap(keyAccessor, hash.keyAccessor);
    swap(hashFunction, hash.hashFunction);
    swap(areEqual, hash.areEqual);
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline void Hash<Item, Key, KeyAccessor, Function, Equal>::empty()
{
	table.becomeEmptyWithSize(MIN_TABLE_SIZE);
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
void Hash<Item, Key, KeyAccessor, Function, Equal>::add(Item& item)
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

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline void Hash<Item, Key, KeyAccessor, Function, Equal>::extendIfFillingUp()
{
	if (isFillingUp())
	{
		rehashItemsInTableWithSize(GROWTH_RATE * table.size());
	}
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline bool Hash<Item, Key, KeyAccessor, Function, Equal>::isFillingUp()
const noexcept
{
	return 3 * table.occupiedSlotsCount() >= 2 * table.size();
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline std::size_t
Hash<Item, Key, KeyAccessor, Function, Equal>::hashValueFor(const Key& key)
const noexcept
{
	return hashFunction(key) % table.size();
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline std::size_t
Hash<Item, Key, KeyAccessor, Function, Equal>::nextPositionToProbe(std::size_t currentPosition)
const noexcept
{
	return (currentPosition + 1) % table.size();
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
void Hash<Item, Key, KeyAccessor, Function, Equal>::rehashItemsInTableWithSize(std::size_t size)
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

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
void Hash<Item, Key, KeyAccessor, Function, Equal>::addAllItemsFrom(Table& table)
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

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
Item& Hash<Item, Key, KeyAccessor, Function, Equal>::operator[](const Key& key)
{
	const auto& hash = *this;

	return const_cast<Item&>(hash[key]);
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
const Item& Hash<Item, Key, KeyAccessor, Function, Equal>::operator[](const Key& key) const
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

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
long Hash<Item, Key, KeyAccessor, Function, Equal>::indexOfFirstItemWithKey(const Key& key)
const noexcept
{
	auto index = hashValueFor(key);

	while (table.isOccupiedAt(index) &&
           !areEqual(keyAccessor(table[index]), key))
	{
		index = nextPositionToProbe(index);
	}

	return table.isOccupiedAt(index) ? index : -1;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline bool Hash<Item, Key, KeyAccessor, Function, Equal>::contains(const Key& key)
const noexcept
{
	return indexOfFirstItemWithKey(key) != -1;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
Item* Hash<Item, Key, KeyAccessor, Function, Equal>::remove(const Key& key) noexcept
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

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline bool Hash<Item, Key, KeyAccessor, Function, Equal>::hasTooManyEmptySlots()
const noexcept
{
	return 6 * table.occupiedSlotsCount() <= table.size();
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline bool Hash<Item, Key, KeyAccessor, Function, Equal>::canBeShrinked()
const noexcept
{
	return (table.size() / GROWTH_RATE) >= MIN_TABLE_SIZE;
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
void Hash<Item, Key, KeyAccessor, Function, Equal>::shrinkAfterRemovingItemAt(std::size_t index)
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

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
void Hash<Item, Key, KeyAccessor, Function, Equal>::rehashClusterStartingAt(
    std::size_t index
) noexcept
{
	while (table.isOccupiedAt(index))
	{
		add(*(table.extractItemAt(index)));
		index = nextPositionToProbe(index);
	}
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline std::size_t Hash<Item, Key, KeyAccessor, Function, Equal>::getSize() const noexcept
{
	return table.occupiedSlotsCount();
}

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline bool Hash<Item, Key, KeyAccessor, Function, Equal>::isEmpty() const noexcept
{
	return getSize() == 0;
}
