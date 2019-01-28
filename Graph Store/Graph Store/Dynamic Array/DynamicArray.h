#ifndef __DYNAMIC_ARRAY_HEADER_INCLUDED__
#define __DYNAMIC_ARRAY_HEADER_INCLUDED__

#include <type_traits>

template <class T>
class DynamicArray
{
public:
	template <class Item, bool isConst = false>
	class DynamicArrayIterator
	{
		friend class DynamicArray<Item>;
		typedef size_t Position;
		typedef typename std::conditional<isConst, const DynamicArray<Item>*, DynamicArray<Item>*>::type OwnerPtr;

	public:
		typedef typename std::conditional<isConst, const Item&, Item&>::type Reference;
		typedef typename std::conditional<isConst, const Item*, Item*>::type Pointer;

	public:
		DynamicArrayIterator(const DynamicArrayIterator<Item, false>& source) noexcept;

		DynamicArrayIterator<Item, isConst>& operator++() noexcept;
		DynamicArrayIterator<Item, isConst> operator++(int) noexcept;
		Reference operator*() const;
		Pointer operator->() const;
		bool operator!() const noexcept;
		operator bool() const noexcept;

		template <class Item, bool isConst>
		friend bool operator==(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
							   typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs) noexcept;

	private:
		DynamicArrayIterator(Position currentPosition, OwnerPtr owner);

		Reference getCurrentItem() const;
		bool isValid() const noexcept;

	private:
		Position currentPosition;
		OwnerPtr owner;
	};

	typedef DynamicArrayIterator<T, false> Iterator;
	typedef DynamicArrayIterator<T, true> ConstIterator;

public:
	explicit DynamicArray(size_t size = 0, size_t count = 0);
	DynamicArray(DynamicArray<T>&& source);
	DynamicArray(const DynamicArray<T>& source);
	DynamicArray<T>& operator=(DynamicArray<T>&& rhs);
	DynamicArray<T>& operator=(const DynamicArray<T>& rhs);
	~DynamicArray();

	void add(const T& item);
	void removeAt(size_t index);
	void removeLast();

	void addAt(size_t index, const T& item);
	void ensureSize(size_t size);
	void empty();
	bool isEmpty() const;

	size_t getSize() const;
	size_t getCount() const;

	Iterator getIterator();
	ConstIterator getConstIterator() const;

public:
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	DynamicArray<T>& operator+=(const T& item);
	DynamicArray<T>& operator+=(const DynamicArray<T>& rhs);

private:
	void shiftLeft(size_t first, size_t last);
	void shiftRight(size_t first, size_t last);
	void extendIfFull();
	void resize(size_t newSize);
	void copyFrom(const DynamicArray<T>& source);
	void swapContentsWith(DynamicArray<T> other);
	void destroyItems();
	void nullifyMembers();
	void validateIndex(size_t i) const;
	void setCount(size_t newCount);

private:
	static const size_t GROWTH_RATE = 2;

private:
	size_t count;
	size_t size;
	T* items;
};

template <class Item, bool isConst>
bool operator!=(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
				typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs) noexcept;

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template <class T>
DynamicArray<T> operator+(const T& item, const DynamicArray<T>& arr);

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& arr, const T& item);

#include "DynamicArrayIterator.hpp"
#include "DynamicArray.hpp"

#endif //__DYNAMIC_ARRAY_HEADER_INCLUDED__