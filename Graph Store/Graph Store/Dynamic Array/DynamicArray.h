#ifndef __DYNAMIC_ARRAY_HEADER_INCLUDED__
#define __DYNAMIC_ARRAY_HEADER_INCLUDED__

#include "../Iterator/Iterator.h"

template <class T>
class DynamicArray
{
public:
	template <class Item>
	class DynamicArrayIterator : public Iterator<Item>
	{
		friend class DynamicArray<Item>;
		typedef size_t Position;

	public:
		virtual ~DynamicArrayIterator() = default;

		virtual DynamicArrayIterator<Item>& operator++() override;
		virtual Item& operator*() override;
		virtual Item* operator->() override;
		virtual bool operator!() const override;
		virtual operator bool() const override;

		template <class Item>
		friend bool operator==(typename const DynamicArray<Item>::DynamicArrayIterator<Item>& lhs,
							   typename const DynamicArray<Item>::DynamicArrayIterator<Item>& rhs);

	private:
		DynamicArrayIterator(Position currentPosition, DynamicArray<Item>* owner);

		Item& getCurrentItem();
		bool isValid() const;

	private:
		Position currentPosition;
		DynamicArray<Item>* owner;
	};

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

	DynamicArrayIterator<T> getIteratorToFirst();

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

private:
	static const size_t GROWTH_RATE = 2;

private:
	size_t count;
	size_t size;
	T* items;
};

template <class Item>
bool operator!=(typename const DynamicArray<Item>::DynamicArrayIterator<Item>& lhs,
				typename const DynamicArray<Item>::DynamicArrayIterator<Item>& rhs);

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template <class T>
DynamicArray<T> operator+(const T& item, const DynamicArray<T>& arr);

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& arr, const T& item);

#include "DynamicArrayIterator.hpp"
#include "DynamicArray.hpp"

#endif //__DYNAMIC_ARRAY_HEADER_INCLUDED__