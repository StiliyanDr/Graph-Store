#ifndef __HASH_HEADER_INCLUDED__
#define __HASH_HEADER_INCLUDED__

#include "DArray.h"
#include "HashFunction.h"

template <class Item, class Key, class KeyAccessor>
class Hash
{
public:
	explicit Hash(int expectedCount);
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
	static const int MIN_TABLE_SIZE = 3;
	static const int SEARCH_MISS = -1;

private:
	size_t count;			 
	DArray<Item*> table;
	KeyAccessor keyAccessor;
	HashFunction<Key> hashFunction;

private:
	int searchAndGetIndex(const Key& key);
	void resize(int newSize);
	void rehashCluster(int start);
	void swapContentsWith(Hash<Item, Key, KeyAccessor> temp);

private:
	static void nullify(DArray<Item*>& table);
	static int calculateTableSize(int expectedCount);
};

#include "Hash.hpp"

#endif //__HASH_HEADER_INCLUDED__