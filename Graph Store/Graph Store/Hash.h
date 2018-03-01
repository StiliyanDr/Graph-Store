#ifndef __HASH_HEADER_INCLUDED__
#define __HASH_HEADER_INCLUDED__

#include "DArray.h"
#include "HashFunction.h"

template <class Item, class Key, class KeyAccessor>
class Hash
{
public:
	explicit Hash(int expectedCount);

	Item* search(const Key& key);
	void insert(Item& item);
	Item* remove(const Key& key);

private:
	size_t count;			   //The number of elements in the table.
	DArray<Item*> table;	   //The table of pointers to store Item addresses. 
	KeyAccessor accessor;      //A functor that gives access to an item's key.
	HashFunction<Key> hash;    //The hash function.

private:
	int getIndex(const Key& key) const;
};

#endif //__HASH_HEADER_INCLUDED__