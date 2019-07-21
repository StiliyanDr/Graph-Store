#include <assert.h>
#include <stdexcept>
#include "Utility.h"

template <class T>
inline DynamicArray<T>::DynamicArray() noexcept :
	DynamicArray(0, 0)
{
}

template <class T>
DynamicArray<T>::DynamicArray(SizeType capacity, SizeType size) :
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
void DynamicArray<T>::setSize(SizeType newSize)
{
	if (newSize <= capacity)
	{
		size = newSize;
	}
	else
	{
		throw std::invalid_argument{ "Size must not exceed capacity!" };
	}
}

template <class T>
DynamicArray<T>::DynamicArray(DynamicArray&& source) noexcept :
	size(source.size),
	capacity(source.capacity),
	items(source.items)
{
	source.nullifyMembers();
}

template <class T>
inline DynamicArray<T>::DynamicArray(const DynamicArray& source) :
	DynamicArray{}
{
	copyFrom(source);
}

template <class T>
void DynamicArray<T>::copyFrom(const DynamicArray& source)
{
	auto theCopy = DynamicArray(source.capacity, source.size);

	for (auto i = 0u; i < source.size; ++i)
	{
		theCopy.items[i] = source.items[i];
	}

	swapContentsWith(std::move(theCopy));
}

template <class T>
void DynamicArray<T>::swapContentsWith(DynamicArray other) noexcept
{
	std::swap(size, other.size);
	std::swap(capacity, other.capacity);
	std::swap(items, other.items);
}

template <class T>
DynamicArray<T>&
DynamicArray<T>::operator=(DynamicArray&& rhs) noexcept
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class T>
DynamicArray<T>&
DynamicArray<T>::operator=(const DynamicArray& rhs)
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
void DynamicArray<T>::resize(SizeType newCapacity)
{
	auto newSize = (newCapacity < size) ? newCapacity : size;
	auto newArray = DynamicArray(newCapacity, newSize);

	for (auto i = 0u; i < newSize; ++i)
	{
		newArray.items[i] = moveAssignIfNoexcept(items[i]);
	}

	swapContentsWith(std::move(newArray));
}

template <class T>
inline void DynamicArray<T>::addAt(SizeType index,
	                               const T& item)
{
	doAddAt(index, item);
}

template <class T>
inline void DynamicArray<T>::addAt(SizeType index,
	                               T&& item)
{
	doAddAt(index, std::move(item));
}

template <class T>
template <class U>
void DynamicArray<T>::doAddAt(SizeType index, U&& item)
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
		throw std::out_of_range{ "Index out of range!" };
	}
}

template <class T>
void DynamicArray<T>::shiftRight(SizeType first,
	                             SizeType last) noexcept
{
	assert(last + 1 < capacity);

	for (auto i = last + 1; i > first; --i)
	{
		items[i] = moveAssignIfNoexcept(items[i - 1]);
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
		throw std::logic_error{ "There is no last element!" };
	}
}

template <class T>
inline void DynamicArray<T>::removeAt(SizeType index)
{
	validateIndex(index);

	shiftLeft(index + 1, --size);
}

template <class T>
void DynamicArray<T>::shiftLeft(SizeType first,
	                            SizeType last) noexcept
{
	assert(first > 0);
	assert(last < capacity);

	for (auto i = first - 1; i < last; ++i)
	{
		items[i] = moveAssignIfNoexcept(items[i + 1]);
	}
}

template <class T>
inline T& DynamicArray<T>::operator[](SizeType index)
{
	const auto& arr = *this;

	return const_cast<T&>(arr[index]);
}

template <class T>
const T&
DynamicArray<T>::operator[](SizeType index) const
{
	validateIndex(index);

	return items[index];
}

template <class T>
inline void DynamicArray<T>::reserve(SizeType capacity)
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
inline auto DynamicArray<T>::getIterator() noexcept
-> Iterator
{
	return Iterator(0, this);
}

template <class T>
inline auto DynamicArray<T>::getConstIterator() const noexcept
-> ConstIterator
{
	return ConstIterator(0, this);
}

template <class T>
inline void DynamicArray<T>::nullifyMembers() noexcept
{
	items = nullptr;
	capacity = size = 0;
}

template <class T>
inline void DynamicArray<T>::validateIndex(SizeType i) const
{
	assert(i >= 0);

	if (i >= size)
	{
		throw std::out_of_range{ "Index out of range!" };
	}
}

template <class T>
inline bool DynamicArray<T>::isEmpty() const noexcept
{
	return size == 0;
}

template <class T>
inline auto DynamicArray<T>::getSize() const noexcept
-> SizeType
{
	return size;
}

template <class T>
inline auto DynamicArray<T>::getCapacity() const noexcept
-> SizeType
{
	return capacity;
}

template <class T>
inline void DynamicArray<T>::destroyItems() noexcept
{
	delete[] items;
}
