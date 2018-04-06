#ifndef __HASH_HEADER_INCLUDED__
#define __HASH_HEADER_INCLUDED__

#include "DArray.h"
#include "HashFunction.h"

template <class Item, class Key, class KeyAccessor>
class Hash
{
public:
	explicit Hash(size_t expectedCount);
	Hash(Hash<Item, Key, KeyAccessor>&& source);
	Hash(const Hash<Item, Key, KeyAccessor>& source) = default;
	Hash<Item, Key, KeyAccessor>& operator=(Hash<Item, Key, KeyAccessor>&& rhs);
	Hash<Item, Key, KeyAccessor>& operator=(const Hash<Item, Key, KeyAccessor>& rhs);
	~Hash() = default;

	Item* search(const Key& key);
	void add(Item& item);
	Item* remove(const Key& key);

	size_t getCount() const;
	bool isEmpty() const;
	void empty();

private:
	static const size_t MIN_TABLE_SIZE = 3;
	static const long SEARCH_MISS = -1;

private:
	size_t count, tableSize;			 
	DArray<Item*> table;
	KeyAccessor keyAccessor;
	HashFunction<Key> hashFunction;

private:
	long searchAndGetIndex(const Key& key);
	void resize(size_t newSize);
	void rehashCluster(size_t start);
	Item* emptySlotAndReturnItemAt(size_t index);
	bool hasTooManyEmptySlots() const;
	bool tableCanBeHalved() const;
	bool isFillingUp() const;
	void swapContentsWith(Hash<Item, Key, KeyAccessor> temp);

private:
	static void nullify(DArray<Item*>& table);
	static size_t calculateTableSize(size_t expectedCount);
};

#include "Hash.hpp"

#endif //__HASH_HEADER_INCLUDED__