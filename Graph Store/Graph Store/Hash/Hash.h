#ifndef __HASH_HEADER_INCLUDED__
#define __HASH_HEADER_INCLUDED__

#include "Dynamic Array/DynamicArray.h"
#include "Hash Function/HashFunction.h"

class Identity
{
public:
	template <class T>
	const T& operator()(const T& object) const noexcept
	{
		return object;
	}
};

template <class Item,
	class Key = Item,
	class KeyAccessor = Identity,
	class Function = HashFunction<Key>>
class Hash
{
	class Table
	{
	public:
		explicit Table(std::size_t size = 0);
		Table(const Table&) = default;
		Table& operator=(const Table&) = default;
		Table(Table&& source) noexcept;
		Table& operator=(Table&& rhs) noexcept;

		void addAt(std::size_t index, Item& item) noexcept;
		Item* extractItemAt(std::size_t index) noexcept;
		void becomeEmptyWithSize(std::size_t size);
		bool isOccupiedAt(std::size_t index) const noexcept;

		std::size_t occupiedSlotsCount() const noexcept;
		std::size_t size() const noexcept;

		Item& operator[](std::size_t index) noexcept;
		const Item& operator[](std::size_t index) const noexcept;

	private:
		void swapContentsWith(Table t) noexcept;

	private:
		DynamicArray<Item*> slots;
		std::size_t count;
	};

public:
	explicit Hash(std::size_t expectedItemsCount = 0);
	Hash(const Hash<Item, Key, KeyAccessor, Function>&) = default;
	Hash<Item, Key, KeyAccessor, Function>& operator=(const Hash<Item, Key, KeyAccessor, Function>&) = default;
	Hash(Hash<Item, Key, KeyAccessor, Function>&& source);
	Hash<Item, Key, KeyAccessor, Function>& operator=(Hash<Item, Key, KeyAccessor, Function>&& rhs);

	void add(Item& item);
	Item* remove(const Key& key);
	bool contains(const Key& key) const;

	Item& operator[](const Key& key);
	const Item& operator[](const Key& key) const;

	std::size_t getCount() const noexcept;
	bool isEmpty() const noexcept;
	void empty();
	void swap(Hash<Item, Key, KeyAccessor, Function>& hash) noexcept;

private:
	static std::size_t calculateTableSize(std::size_t expectedItemsCount);

private:
	long getIndexOfFirstItemWithKey(const Key& key) const;
	std::size_t computeIndexFromKey(const Key& key) const;
	void rehashClusterStartingAt(std::size_t index);
	void shrinkAfterRemovingItemAt(std::size_t index);
	void rehashItemsInTableWithSize(std::size_t newSize);
	void addAllItemsFrom(Table& table);
	bool hasTooManyEmptySlots() const;
	bool canBeShrinked() const;
	void extendIfFillingUp();
	bool isFillingUp() const;
	std::size_t getNextPositionToProbe(std::size_t currentPosition) const;

private:
	static const std::size_t GROWTH_RATE = 2;
	static const std::size_t MIN_TABLE_SIZE = 3;

private:
	Table table;
	KeyAccessor keyAccessor;
	Function hashFunction;
};

template <class Item, class Key, class KeyAccessor, class Function>
inline void swap(Hash<Item, Key, KeyAccessor, Function>& lhs,
	             Hash<Item, Key, KeyAccessor, Function>& rhs) noexcept
{
	lhs.swap(rhs);
}

#include "Hash.hpp"

#endif //__HASH_HEADER_INCLUDED__