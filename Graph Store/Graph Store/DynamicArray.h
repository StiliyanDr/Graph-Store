#ifndef __DYNAMIC_ARRAY_HEADER_INCLUDED__
#define __DYNAMIC_ARRAY_HEADER_INCLUDED__

template <class T>
class DynamicArrayIterator;

template <class T>
class DynamicArray
{
public:
	explicit DynamicArray(size_t size = 0, size_t count = 0);
	DynamicArray(DynamicArray<T>&& source);
	DynamicArray(const DynamicArray<T>& source);
	DynamicArray<T>& operator=(DynamicArray<T>&& rhs);
	DynamicArray<T>& operator=(const DynamicArray<T>& rhs);
	virtual ~DynamicArray();

	virtual void add(const T& item);
	virtual void remove(size_t index);
	virtual long search(const T& what) const;

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
	void swapContentsWith(DynamicArray<T> temp);
	void destroyItems();
	void nullifyMembers();

private:
	size_t count, size;
	T* items;
};

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template <class T>
DynamicArray<T> operator+(const T& lhs, const DynamicArray<T>& rhs);

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& lhs, const T& rhs);

template <class T>
class DynamicArrayIterator
{
	typedef DynamicArrayIterator<T> Iterator;
	typedef long Position;

	template <class T>
	friend class DynamicArray;

public:
	Iterator& operator++();
	Iterator operator++(int);

	T& operator*();
	bool operator!() const;
	operator bool() const;

	template <class T>
	friend bool operator==(const DynamicArrayIterator<T>& lhs, const DynamicArrayIterator<T>& rhs);

	template <class T>
	friend bool operator!=(const DynamicArrayIterator<T>& lhs, const DynamicArrayIterator<T>& rhs);

private:
	DynamicArrayIterator(Position current, DynamicArray<T>* owner);
	bool isValid() const;

private:
	static const Position INVALID_POSITION = -1;

private:
	Position current;
	DynamicArray<T>* owner;
};

#include "DynamicArrayIterator.hpp"
#include "DynamicArray.hpp"

#endif //__DYNAMIC_ARRAY_HEADER_INCLUDED__