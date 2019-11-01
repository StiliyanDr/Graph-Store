#ifndef __HASH_HEADER_INCLUDED__
#define __HASH_HEADER_INCLUDED__

#include "Dynamic Array/DynamicArray.h"
#include "Hash Function/HashFunction.h"
#include "Utility.h"

template <
    class Item,
	class Key = Item,
	class KeyAccessor = Identity,
	class Function = HashFunction<Key>,
    class Equal = EqualTo<Key>
>
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
	explicit Hash(std::size_t expectedItemsCount = 0,
                  KeyAccessor accessor = {},
                  Function hashFunction = {},
                  Equal areEqual = {});
	Hash(const Hash&) = default;
	Hash& operator=(const Hash&) = default;
	Hash(Hash&& source);
	Hash& operator=(Hash&& rhs);

	void add(Item& item);
	Item* remove(const Key& key) noexcept;
	bool contains(const Key& key) const noexcept;

	Item& operator[](const Key& key);
	const Item& operator[](const Key& key) const;

	std::size_t getSize() const noexcept;
	bool isEmpty() const noexcept;
	void empty();
	void swap(Hash& hash) noexcept;

private:
	static std::size_t
        calculateTableSize(std::size_t expectedItemsCount) noexcept;

private:
	long indexOfFirstItemWithKey(const Key& key) const noexcept;
	std::size_t hashValueFor(const Key& key) const noexcept;
	void rehashClusterStartingAt(std::size_t index) noexcept;
	void shrinkAfterRemovingItemAt(std::size_t index);
	void rehashItemsInTableWithSize(std::size_t size);
	void addAllItemsFrom(Table& table);
	bool hasTooManyEmptySlots() const noexcept;
	bool canBeShrinked() const noexcept;
	void extendIfFillingUp();
	bool isFillingUp() const noexcept;
	std::size_t nextPositionToProbe(std::size_t currentPosition) const noexcept;

private:
	static const std::size_t GROWTH_RATE = 2;
	static const std::size_t MIN_TABLE_SIZE = 3;

private:
	Table table;
	KeyAccessor keyAccessor;
	Function hashFunction;
    Equal areEqual;
};

template <class Item, class Key, class KeyAccessor, class Function, class Equal>
inline void swap(Hash<Item, Key, KeyAccessor, Function, Equal>& lhs,
	             Hash<Item, Key, KeyAccessor, Function, Equal>& rhs) noexcept
{
	lhs.swap(rhs);
}

#include "Hash.hpp"

#endif //__HASH_HEADER_INCLUDED__