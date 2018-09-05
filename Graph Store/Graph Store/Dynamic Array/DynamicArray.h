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

	public:
		typedef typename std::conditional<isConst, const Item&, Item&>::type Reference;
		typedef typename std::conditional<isConst, const Item*, Item*>::type Pointer;

	public:
		DynamicArrayIterator(const DynamicArrayIterator<Item, false>& source);

		DynamicArrayIterator<Item, isConst>& operator++();
		DynamicArrayIterator<Item, isConst> operator++(int);
		Reference operator*() const;
		Pointer operator->() const;
		bool operator!() const;
		operator bool() const;

		template <class Item, bool isConst>
		friend bool operator==(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
							   typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs);

	private:
		DynamicArrayIterator(Position currentPosition, DynamicArray<Item>* owner);

		Reference getCurrentItem() const;
		bool isValid() const;

	private:
		Position currentPosition;
		DynamicArray<Item>* owner;
	};

	typedef typename DynamicArray<T>::DynamicArrayIterator<T, false> Iterator;
	typedef typename DynamicArray<T>::DynamicArrayIterator<T, true> ConstIterator;

public:
	explicit DynamicArray(size_t size = 0, size_t count = 0);
	DynamicArray(DynamicArray<T>&& source);
	DynamicArray(const DynamicArray<T>& source);
	DynamicArray<T>& operator=(DynamicArray<T>&& rhs);
	DynamicArray<T>& operator=(const DynamicArray<T>& rhs);
	virtual ~DynamicArray();

	virtual void add(const T& item);
	virtual void removeAt(size_t index);

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

protected:
	void extendIfFull();
	void shiftLeft(size_t first, size_t last);
	void shiftRight(size_t first, size_t last);
	void setCount(size_t newCount);
	T* getItems();

private:
	void resize(size_t newSize);
	void copyFrom(const DynamicArray<T>& source);
	void swapContentsWith(DynamicArray<T> other);
	void destroyItems();
	void nullifyMembers();
	void validateIndex(size_t i) const;

private:
	static const size_t GROWTH_RATE = 2;

private:
	size_t count;
	size_t size;
	T* items;
};

template <class Item, bool isConst>
bool operator!=(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
				typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs);

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template <class T>
DynamicArray<T> operator+(const T& item, const DynamicArray<T>& arr);

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& arr, const T& item);

#include "DynamicArrayIterator.hpp"
#include "DynamicArray.hpp"

#endif //__DYNAMIC_ARRAY_HEADER_INCLUDED__