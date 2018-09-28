#ifndef __HASH_HEADER_INCLUDED__
#define __HASH_HEADER_INCLUDED__

#include "../Dynamic Array/DynamicArray.h"
#include "Hash Function/HashFunction.h"

class Identity
{
public:
	template <class T>
	const T& operator()(const T& object) const
	{
		return object;
	}
};

template <class Item, class Key = Item, class Function = HashFunction<Key>, class KeyAccessor = Identity>
class Hash
{
	class Table
	{
	public:
		Table(size_t size = 0);
		Table(const Table&) = default;
		Table& operator=(const Table&) = default;
		Table(Table&& source);
		Table& operator=(Table&& rhs);
		~Table() = default;

		void addAt(size_t index, Item& item);
		Item* extractItemAt(size_t index);
		void becomeEmptyWithSize(size_t size);
		bool isOccupiedAt(size_t index) const;

		size_t occupiedSlotsCount() const;
		size_t size() const;

		Item& operator[](size_t index);
		const Item& operator[](size_t index) const;

	private:
		void swapContentsWith(Table table);

	private:
		DynamicArray<Item*> slots;
		size_t count;
	};

public:
	explicit Hash(size_t expectedItemsCount = 0);
	Hash(const Hash<Item, Key, Function, KeyAccessor>&) = default;
	Hash<Item, Key, Function, KeyAccessor>& operator=(const Hash<Item, Key, Function, KeyAccessor>&) = default;
	Hash(Hash<Item, Key, Function, KeyAccessor>&& source);
	Hash<Item, Key, Function, KeyAccessor>& operator=(Hash<Item, Key, Function, KeyAccessor>&& rhs);
	~Hash() = default;

	void add(Item& item);
	Item* remove(const Key& key);
	bool contains(const Key& key) const;

	Item& operator[](const Key& key);
	const Item& operator[](const Key& key) const;

	size_t getCount() const;
	bool isEmpty() const;
	void empty();

private:
	static size_t calculateTableSize(size_t expectedItemsCount);

private:
	long getIndexOfFirstItemWithKey(const Key& key) const;
	size_t computeIndexFromKey(const Key& key) const;
	void rehashClusterStartingAt(size_t index);
	void shrinkAfterRemovingItemAt(size_t index);
	void resize(size_t newSize);
	void addAllItemsFrom(Table& table);
	bool hasTooManyEmptySlots() const;
	bool tableCanBeShrinked() const;
	void extendIfFillingUp();
	bool isFillingUp() const;
	void swapContentsWith(Hash<Item, Key, Function, KeyAccessor> hash);
	size_t getNextPositionToProbe(size_t currentPosition) const;

private:
	static const size_t GROWTH_RATE = 2;
	static const size_t MIN_TABLE_SIZE = 3;

private:
	Table table;
	KeyAccessor keyAccessor;
	Function hashFunction;
};

#include "Hash.hpp"

#endif //__HASH_HEADER_INCLUDED__