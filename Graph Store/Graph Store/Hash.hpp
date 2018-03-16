#include <cassert>
#include <utility>
#include <stdexcept>


///
/// Allocates an array of null pointers (table slots) big enough to hold
/// expectedCount items and have some spare space to terminate probes.
/// 
/// A Hash object should store at least one item, so expectedCount
/// must be at least 1.
///
/// (!) HashFunction<Key> and KeyAccessor must be default constructable. 
///
template <class Item, class Key, class KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(int expectedCount) :
	count(0),
	table(((3 * expectedCount) / 2) + MIN_TABLE_SIZE, ((3 * expectedCount) / 2) + MIN_TABLE_SIZE)
{
	if (expectedCount < 1)
		throw std::invalid_argument("The expected count must be positive!");

	nullify(table);
}


template <class Item, class Key, class KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(Hash<Item, Key, KeyAccessor>&& source) :
	count(source.count),
	keyAccessor(std::move(source.keyAccessor)),
	hashFunction(std::move(source.hashFunction))
{
	stealTableFrom(source);
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
	int index = searchAndGetIndex(key);

	return (index != SEARCH_MISS) ? table[index] : nullptr;
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::add(Item& item)
{
	size_t tableSize = table.getCount();

	assert(tableSize >= MIN_TABLE_SIZE && count < tableSize);

	if (3 * count >= 2 * tableSize)
	{
		resize(2 * tableSize);
		tableSize = table.getCount();
	}

	int index = hashFunction(keyAccessor(item)) % tableSize;

	while (table[index])
		index = (index + 1) % tableSize;

	table[index] = &item;
	++count;
}


template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::remove(const Key& key)
{
	Item* removed = nullptr;

	int index = searchAndGetIndex(key);

	if (index != SEARCH_MISS)
	{
		assert(count > 0 && table.getCount() > count);

		removed = table[index];  
		table[index] = nullptr; 
		--count;

		const int tableSize = table.getCount();

		if (6 * count <= tableSize && (tableSize / 2) >= MIN_TABLE_SIZE)
			resize(tableSize / 2);
		else
			rehashCluster((index + 1) % tableSize);
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

	for (int i = 1; i <= MIN_TABLE_SIZE; ++i)
		table.add(nullptr);

	count = 0;
}


///
/// (!) hashFunction's operator() must take an object of type Key by
/// const& and return its hash value (unsigned integer).
///
/// (!) keyAccessor's operator() must take an object of type Item
/// by const& and return its key of type Key by const&.
///
/// (!) Key must have overloaded operator!= .
///
template <class Item, class Key, class KeyAccessor>
int Hash<Item, Key, KeyAccessor>::searchAndGetIndex(const Key& key)
{
	if (!isEmpty())
	{
		const int tableSize = table.getCount();

		assert(tableSize >= MIN_TABLE_SIZE);

		int index = hashFunction(key) % tableSize;

		while (table[index] && keyAccessor(*table[index]) != key)
			index = (index + 1) % tableSize;
		
		if (table[index]) 
			return index;
	}

	return SEARCH_MISS;
}


///
/// Resizes the table to have newSize slots. newSize must 
/// be > count so that the table can contain all the current 
/// items and have at least one empty slot to terminate probing.
///
template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::resize(int newSize)
{
	assert(newSize >= MIN_TABLE_SIZE && newSize > count);

	DArray<Item*> temp(newSize, newSize);
	nullify(temp);

	//Swap the current table with the new one by moving them.
	std::swap(table, temp);

	//Items are going to be inserted into the new empty table.
	count = 0;

	int oldTableSize = temp.getCount();

	for (int i = 0; i < oldTableSize; ++i)
	{
		if (temp[i])
			add(*temp[i]);
	}
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::nullify(DArray<Item*>& arr)
{
	int size = arr.getCount();

	for (int i = 0; i < size; ++i)
		arr[i] = nullptr;
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::rehashCluster(int start)
{
	int index = start;
	const int tableSize = table.getCount();

	assert(index >= 0 && index < tableSize);

	Item* temp;

	while (table[index])
	{
		temp = table[index];
		table[index] = nullptr;

		--count;

		add(*temp);
		index = (index + 1) % tableSize;
	}
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::swapContentsWith(Hash<Item, Key, KeyAccessor> temp)
{
	std::swap(count, temp.count);
	std::swap(table, temp.table);
	std::swap(keyAccessor, temp.keyAccessor);
	std::swap(hashFunction, temp.hashFunction);
}


template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::stealTableFrom(Hash<Item, Key, KeyAccessor>& other)
{	
	this->table = std::move(other.table);

	try
	{
		other.empty();
	}
	catch (std::bad_alloc&)
	{
		other.count = this->count;
		other.table = std::move(this->table);
		other.keyAccessor = std::move(this->keyAccessor);
		other.hashFunction = std::move(this->hashFunction);

		throw;
	}
}
