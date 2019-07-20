#ifndef __DYNAMIC_ARRAY_HEADER_INCLUDED__
#define __DYNAMIC_ARRAY_HEADER_INCLUDED__

#include <type_traits>
#include "Iterator/Iterator.h"

template <class T>
class DynamicArray
{
	static_assert((std::is_nothrow_copy_constructible_v<T>
		           && std::is_nothrow_copy_assignable_v<T>)
		          ||
		          (std::is_nothrow_move_constructible_v<T>
			       && std::is_nothrow_move_assignable_v<T>),
		          "T must have safe copy or safe move operations!");

public:
	using SizeType = std::size_t;

	template <class Item, bool isConst = false>
	class DynamicArrayIterator : public AbstractIterator<Item, isConst>
	{
		friend class DynamicArray<Item>;

		using Position = SizeType;
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
	explicit DynamicArray(SizeType capacity, SizeType size = 0u);
	DynamicArray(DynamicArray&& source) noexcept;
	DynamicArray(const DynamicArray& source);
	DynamicArray& operator=(DynamicArray&& rhs) noexcept;
	DynamicArray& operator=(const DynamicArray& rhs);
	~DynamicArray();

	void add(const T& item);
	void add(T&& item);
	void removeAt(SizeType index);
	void removeLast();

	void addAt(SizeType index, const T& item);
	void addAt(SizeType index, T&& item);
	void reserve(SizeType capacity);
	void empty() noexcept;
	bool isEmpty() const noexcept;

	SizeType getCapacity() const noexcept;
	SizeType getSize() const noexcept;

	Iterator getIterator() noexcept;
	ConstIterator getConstIterator() const noexcept;

public:
	T& operator[](SizeType index);
	const T& operator[](SizeType index) const;

private:
	template <class U>
	void doAdd(U&& item);
	template <class U>
	void doAddAt(SizeType index, U&& item);
	void shiftLeft(SizeType first, SizeType last);
	void shiftRight(SizeType first, SizeType last);
	void extendIfFull();
	void resize(SizeType newCapacity);
	void copyFrom(const DynamicArray& source);
	void swapContentsWith(DynamicArray other) noexcept;
	void destroyItems() noexcept;
	void nullifyMembers() noexcept;
	void validateIndex(SizeType i) const;
	void setSize(SizeType newSize);

private:
	static const SizeType GROWTH_RATE = 2u;

private:
	SizeType size;
	SizeType capacity;
	T* items;
};

template <class Item, bool isConst>
bool operator!=(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
				typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs) noexcept;

#include "DynamicArrayIterator.hpp"
#include "DynamicArray.hpp"

#endif //__DYNAMIC_ARRAY_HEADER_INCLUDED__