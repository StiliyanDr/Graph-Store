#include <cassert>
#include <utility>


///
///See getIndex.
///
template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::search(const Key& key)
{
	int index = getIndex(key);

	return (index != -1) ? table[index] : nullptr;
}


///
/// Inserts item's address into the table.
/// The table doubles its size and rehashes the old items,
/// if at least 2/3 of the slots are occupied. If the object is empty
/// (its table has no slots), the table is resized to have 10 slots. 
///
template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::insert(Item& item)
{
	size_t size = table.getCount();

	//In an empty object size = count = 0.
	//In a non-empty object there must be at least one empty slot!
	assert((count == 0 && size == 0) || count < size);

	if (3 * count >= 2 * size)
	{
		resize(size ? 2 * size : 10);
		size = table.getCount();
	}

	int index = hash(accessor(item)) % size;

	while (table[index])
		index = (index + 1) % size;

	table[index] = &item;
	++count;
}


///
/// Removes the first found item that has a matching key
/// (if any). If the number of items left in the table is 
/// at most 1/6 of the number of slots, the table is resized
/// to half of the number of slots. Otherwise the items between 
/// the removed one and the next empty slot in the table are
/// rehashed.
///
template <class Item, class Key, class KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::remove(const Key& key)
{
	//Search for an item with a matching key.
	int index = getIndex(key);

	//If there is such an item:
	if (index != -1)
	{
		assert(count > 0 && table.getCount() > count);

		//Empty the item's slot.
		table[index] = nullptr;
		--count;

		const int size = table.getCount();

		if (6 * count <= size)
			resize(size / 2);			//Shrink the table so memory is not wasted.
		else
			rehash((index + 1) % size); //Rehash the items between the removed item and the
									    //next empty slot since the removal left an empty slot.
	}
}


///
/// Returns the number of elements that
/// are stored in the hash.
///
template <class Item, class Key, class KeyAccessor>
inline size_t Hash<Item, Key, KeyAccessor>::getCount() const
{
	return count;
}


///
/// Checks whether there are no elements
/// stored in the hash.
///
template <class Item, class Key, class KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isEmpty() const
{
	return count == 0;
}


///
///Empties the array of pointers.
///
template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::empty()
{
	table.empty();
	count = 0;
}


///
/// Returns the index of the first found item with a
/// matching key or -1 if there is no such item in the hash.
///
/// hash's operator() must take an object of type Key by
/// const& and return its hash value (unsigned integer).
///
/// accessor's operator() must take an object of type Item
/// by const& and return its key of type Key by const&.
///
template <class Item, class Key, class KeyAccessor>
int Hash<Item, Key, KeyAccessor>::getIndex(const Key& key)
{
	const int size = table.getCount();

	int index = hash(key) % size;

	while (table[index] && accessor(*table[index]) != key)
		index = (index + 1) % size;

	return (table[index]) ? index : -1;
}


///
/// Resizes the table to have newSize slots. newSize must 
/// be > count so that the table can contain all the current 
/// elements and have at least one empty slot.
///
template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::resize(int newSize)
{
	//Leave at least one empty slot in the table
	//to terminate probing.
	assert(newSize > 0 && newSize > count);

	//Allocate an array with the new size.
	DArray<Item*> temp(newSize, newSize);
	nullify(temp);

	//Swap the current table with the new one by moving them.
	std::swap(table, temp);

	//Elements are going to be re-inserted into the new table,
	//so count must be updated first.
	count = 0;

	//Now rehash the elements from the old table into the new table.
	int oldSize = temp.getCount();

	for (int i = 0; i < oldSize; ++i)
	{
		if (temp[i])
			insert(*temp[i]);
	}
}


///
/// Makes every pointer in the passed
/// array a null pointer.
///
template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::nullify(DArray<Item*>& arr)
{
	int size = arr.getCount();

	for (int i = 0; i < size; ++i)
		arr[i] = nullptr;
}


///
/// Starting from index, the function rehashes all the
/// items in the table, until an empty slot is reached.
///
template <class Item, class Key, class KeyAccessor>
void Hash<Item, Key, KeyAccessor>::rehash(int index)
{
	const int size = table.getCount();

	assert(index >= 0 && index < size);

	//A pointer to the item that is being rehashed.
	Item* temp;

	while (table[index])
	{
		temp = table[index];
		table[index] = nullptr;

		--count;

		insert(*temp);
		index = (index + 1) % size;
	}
}
