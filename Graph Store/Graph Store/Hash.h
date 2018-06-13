#ifndef __HASH_HEADER_INCLUDED__
#define __HASH_HEADER_INCLUDED__

#include "DynamicArray.h"
#include "HashFunction.h"

template <class Item, class Key, class KeyAccessor>
class Hash
{
	typedef DynamicArray<Item*> Table;

public:
	explicit Hash(size_t expectedItemsCount);
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
	static Table createEmptyTableWithSize(size_t size);
	static Table emptyAllSlotsIn(Table table);
	static size_t calculateTableSize(size_t expectedItemsCount);

private:
	long searchAndGetIndex(const Key& key);
	size_t computeIndexFromKey(const Key& key);
	void rehashCluster(size_t start);
	Item* emptySlotAndReturnItemAt(size_t index);
	void resize(size_t newSize);
	void addAllItemsFrom(Table& table);
	bool hasTooManyEmptySlots() const;
	bool tableCanBeShrinked() const;
	void extendIfFillingUp();
	bool isFillingUp() const;
	void makeTableEmptyWithSize(size_t size);
	void setTable(Table table);
	void swapContentsWith(Hash<Item, Key, KeyAccessor> temp);
	size_t getNextPositionToProbe(size_t currentPosition) const;

private:
	static const size_t GROWTH_RATE = 2;
	static const size_t MIN_TABLE_SIZE = 3;
	static const long SEARCH_MISS = -1;

private:
	size_t count;
	size_t tableSize;
	Table table;
	KeyAccessor keyAccessor;
	HashFunction<Key> hashFunction;
};

#include "Hash.hpp"

#endif //__HASH_HEADER_INCLUDED__