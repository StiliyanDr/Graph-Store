#include <assert.h>
#include <stdexcept>
#include "Utility.h"

template <class T>
inline DynamicArray<T>::DynamicArray() noexcept :
	DynamicArray(0, 0)
{
}

template <class T>
DynamicArray<T>::DynamicArray(std::size_t capacity, std::size_t size) :
	capacity(capacity),
	items(nullptr)
{
	setSize(size);

	if (capacity > 0)
	{
		items = new T[capacity];
	}
}

template <class T>
void DynamicArray<T>::setSize(std::size_t newSize)
{
	if (newSize <= capacity)
	{
		size = newSize;
	}
	else
	{
		throw std::invalid_argument("Size must not exceed capacity!");
	}
}

template <class T>
inline DynamicArray<T>::DynamicArray(DynamicArray<T>&& source) noexcept :
	size(source.size),
	capacity(source.capacity),
	items(source.items)
{
	source.nullifyMembers();
}

template <class T>
inline DynamicArray<T>::DynamicArray(const DynamicArray<T>& source) :
	DynamicArray()
{
	copyFrom(source);
}

template <class T>
void DynamicArray<T>::copyFrom(const DynamicArray<T>& source)
{
	DynamicArray<T> theCopy(source.capacity, source.size);

	for (std::size_t i = 0; i < source.size; ++i)
	{
		theCopy.items[i] = source.items[i];
	}

	swapContentsWith(std::move(theCopy));
}

template <class T>
void DynamicArray<T>::swapContentsWith(DynamicArray<T> other)
{
	std::swap(size, other.size);
	std::swap(capacity, other.capacity);
	std::swap(items, other.items);
}

template <class T>
DynamicArray<T>&
DynamicArray<T>::operator=(DynamicArray<T>&& rhs) noexcept
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class T>
DynamicArray<T>&
DynamicArray<T>::operator=(const DynamicArray<T>& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(rhs);
	}

	return *this;
}

template <class T>
inline DynamicArray<T>::~DynamicArray()
{
	destroyItems();
}

template <class T>
inline void DynamicArray<T>::add(const T& item)
{
	doAdd(item);
}

template <class T>
inline void DynamicArray<T>::add(T&& item)
{
	doAdd(std::move(item));
}

template <class T>
template <class U>
void DynamicArray<T>::doAdd(U&& item)
{
	extendIfFull();

	items[size] = std::forward<U>(item);
	++size;
}

template <class T>
void DynamicArray<T>::extendIfFull()
{
	assert(size <= capacity);

	if (size >= capacity)
	{
		resize(capacity > 0 ? GROWTH_RATE * capacity : 2);
	}
}

template <class T>
void DynamicArray<T>::resize(std::size_t newCapacity)
{
	std::size_t newSize = (newCapacity < size) ? newCapacity : size;

	DynamicArray<T> newArray(newCapacity, newSize);

	for (std::size_t i = 0; i < newSize; ++i)
	{
		newArray.items[i] = moveAssignIfNoexcept(items[i]);
	}

	swapContentsWith(std::move(newArray));
}

template <class T>
inline void DynamicArray<T>::addAt(std::size_t index,
	                               const T& item)
{
	doAddAt(index, item);
}

template <class T>
inline void DynamicArray<T>::addAt(std::size_t index,
	                               T&& item)
{
	doAddAt(index, std::move(item));
}

template <class T>
template <class U>
void DynamicArray<T>::doAddAt(std::size_t index, U&& item)
{
	if (index <= size)
	{
		extendIfFull();
		shiftRight(index, size - 1);
		items[index] = std::forward<U>(item);
		++size;
	}
	else
	{
		throw std::out_of_range("Index out of range!");
	}
}

template <class T>
void DynamicArray<T>::shiftRight(std::size_t first,
	                             std::size_t last)
{
	assert(last + 1 < capacity);

	for (std::size_t i = last + 1; i > first; --i)
	{
		items[i] = std::move(items[i - 1]);
	}
}

template <class T>
void DynamicArray<T>::removeLast()
{
	if (!isEmpty())
	{
		removeAt(size - 1);
	}
	else
	{
		throw std::logic_error("There is no last element!");
	}
}

template <class T>
inline void DynamicArray<T>::removeAt(std::size_t index)
{
	validateIndex(index);

	shiftLeft(index + 1, --size);
}

template <class T>
void DynamicArray<T>::shiftLeft(std::size_t first,
	                            std::size_t last)
{
	assert(first > 0);
	assert(last < capacity);

	for (std::size_t i = first - 1; i < last; ++i)
	{
		items[i] = std::move(items[i + 1]);
	}
}

template <class T>
inline T& DynamicArray<T>::operator[](std::size_t index)
{
	const DynamicArray<T>& arr = *this;

	return const_cast<T&>(arr[index]);
}

template <class T>
const T&
DynamicArray<T>::operator[](std::size_t index) const
{
	validateIndex(index);

	return items[index];
}

template <class T>
inline void DynamicArray<T>::reserve(std::size_t capacity)
{
	if (capacity > this->capacity)
	{
		resize(capacity);
	}
}

template <class T>
inline void DynamicArray<T>::empty() noexcept
{
	destroyItems();
	nullifyMembers();
}

template <class T>
inline typename DynamicArray<T>::Iterator
DynamicArray<T>::getIterator() noexcept
{
	return Iterator(0, this);
}

template <class T>
inline typename DynamicArray<T>::ConstIterator
DynamicArray<T>::getConstIterator() const noexcept
{
	return ConstIterator(0, this);
}

template <class T>
inline void DynamicArray<T>::nullifyMembers()
{
	items = nullptr;
	capacity = size = 0;
}

template <class T>
inline void DynamicArray<T>::validateIndex(std::size_t i) const
{
	if (i >= size)
	{
		throw std::out_of_range("Index out of range!");
	}
}

template <class T>
inline bool DynamicArray<T>::isEmpty() const noexcept
{
	return size == 0;
}

template <class T>
inline std::size_t DynamicArray<T>::getSize() const noexcept
{
	return size;
}

template <class T>
inline std::size_t DynamicArray<T>::getCapacity() const noexcept
{
	return capacity;
}

template <class T>
inline void DynamicArray<T>::destroyItems()
{
	delete[] items;
}
