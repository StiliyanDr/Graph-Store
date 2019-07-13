#ifndef __DYNAMIC_ARRAY_HEADER_INCLUDED__
#define __DYNAMIC_ARRAY_HEADER_INCLUDED__

#include <type_traits>
#include "Iterator/Iterator.h"

template <class T>
class DynamicArray
{
public:
	template <class Item, bool isConst = false>
	class DynamicArrayIterator : public AbstractIterator<Item, isConst>
	{
		friend class DynamicArray<Item>;

		using Position = std::size_t;
		using OwnerPtr = std::conditional_t<isConst,
			                                const DynamicArray<Item>*,
			                                DynamicArray<Item>*>;
	public:
		using typename AbstractIterator<Item, isConst>::Reference;
		using typename AbstractIterator<Item, isConst>::Pointer;

	public:
		DynamicArrayIterator(const DynamicArrayIterator<Item, false>& source) noexcept;

		DynamicArrayIterator& operator++() noexcept override;
		const DynamicArrayIterator operator++(int) noexcept;

		template <class Item, bool isConst>
		friend bool operator==(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
							   typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs) noexcept;

	private:
		DynamicArrayIterator(Position currentPosition, OwnerPtr owner) noexcept;

		Reference getCurrentItem() const override;
		bool isValid() const noexcept override;

	private:
		Position currentPosition;
		OwnerPtr owner;
	};

	using Iterator = DynamicArrayIterator<T, false>;
	using ConstIterator = DynamicArrayIterator<T, true>;

public:
	DynamicArray() noexcept;
	explicit DynamicArray(std::size_t size, std::size_t count = 0);
	DynamicArray(DynamicArray<T>&& source) noexcept;
	DynamicArray(const DynamicArray<T>& source);
	DynamicArray<T>& operator=(DynamicArray<T>&& rhs) noexcept;
	DynamicArray<T>& operator=(const DynamicArray<T>& rhs);
	~DynamicArray();

	void add(const T& item);
	void add(T&& item);
	void removeAt(std::size_t index);
	void removeLast();

	void addAt(std::size_t index, const T& item);
	void addAt(std::size_t index, T&& item);
	void ensureSize(std::size_t size);
	void empty() noexcept;
	bool isEmpty() const noexcept;

	std::size_t getSize() const noexcept;
	std::size_t getCount() const noexcept;

	Iterator getIterator() noexcept;
	ConstIterator getConstIterator() const noexcept;

public:
	T& operator[](std::size_t index);
	const T& operator[](std::size_t index) const;

private:
	template <class U>
	void doAdd(U&& item);
	template <class U>
	void doAddAt(std::size_t index, U&& item);
	void shiftLeft(std::size_t first, std::size_t last);
	void shiftRight(std::size_t first, std::size_t last);
	void extendIfFull();
	void resize(std::size_t newSize);
	void copyFrom(const DynamicArray<T>& source);
	void swapContentsWith(DynamicArray<T> other);
	void destroyItems();
	void nullifyMembers();
	void validateIndex(std::size_t i) const;
	void setCount(std::size_t newCount);

private:
	static const std::size_t GROWTH_RATE = 2;

private:
	std::size_t count;
	std::size_t size;
	T* items;
};

template <class Item, bool isConst>
bool operator!=(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
				typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs) noexcept;

#include "DynamicArrayIterator.hpp"
#include "DynamicArray.hpp"

#endif //__DYNAMIC_ARRAY_HEADER_INCLUDED__