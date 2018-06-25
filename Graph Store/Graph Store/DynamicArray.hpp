#include <assert.h>
#include <utility>

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs)
{
	DynamicArray<T> newArray(lhs);
	newArray += rhs;

	return newArray;
}

template <class T>
DynamicArray<T> operator+(const T& item, const DynamicArray<T>& arr)
{
	DynamicArray<T> newArray(arr.getCount() + 1);
	newArray += item;
	newArray += arr;

	return newArray;
}

template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& arr, const T& item)
{
	DynamicArray<T> newArray(arr);
	newArray += item;

	return newArray;
}

template <class T>
DynamicArray<T>::DynamicArray(size_t size, size_t count) :
	size(size), items(nullptr)
{
	setCount(count);

	if (size > 0)
	{
		items = new T[size];
	}
}

template <class T>
inline DynamicArray<T>::DynamicArray(DynamicArray<T>&& source) :
	count(source.count), size(source.size), items(source.items)
{
	source.nullifyMembers();
}

template <class T>
inline DynamicArray<T>::DynamicArray(const DynamicArray<T>& source)
{
	nullifyMembers();
	copyFrom(source);
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& rhs)
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
void DynamicArray<T>::add(const T& item)
{
	extendIfFull();

	items[count++] = item;
}

template <class T>
void DynamicArray<T>::removeAt(size_t index)
{
	assert(index < count);

	shiftLeft(index + 1, --count);
}

template <class T>
long DynamicArray<T>::search(const T& what) const
{
	for (size_t i = 0; i < count; ++i)
	{
		if (items[i] == what)
		{
			return i;
		}
	}

	return -1;
}

template <class T>
void DynamicArray<T>::addAt(size_t index, const T& item)
{
	assert(index <= count);

	extendIfFull();
	shiftRight(index, count - 1);
	items[index] = item;
	++count;
}

template <class T>
inline void DynamicArray<T>::ensureSize(size_t size)
{
	if (size > this->size)
	{
		resize(size);
	}
}

template <class T>
void DynamicArray<T>::empty()
{
	destroyItems();
	nullifyMembers();
}

template <class T>
inline bool DynamicArray<T>::isEmpty() const
{
	return count == 0;
}

template <class T>
inline size_t DynamicArray<T>::getSize() const
{
	return size;
}

template <class T>
inline size_t DynamicArray<T>::getCount() const
{
	return count;
}

template <class T>
DynamicArrayIterator<T> DynamicArray<T>::getIteratorToFirst()
{
	DynamicArrayIterator<T>::Position position;
    position = isEmpty() ? DynamicArrayIterator<T>::INVALID_POSITION : 0;

	return DynamicArrayIterator<T>(position, this);
}

template <class T>
T& DynamicArray<T>::operator[](size_t index)
{
	assert(index < count);

	return items[index];
}

template <class T>
const T& DynamicArray<T>::operator[](size_t index) const
{
	assert(index < count);

	return items[index];
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator+=(const T& item)
{
	add(item);

	return *this;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator+=(const DynamicArray<T>& rhs)
{
	for (size_t i = 0; i < rhs.count; ++i)
	{
		*this += rhs[i];
	}

	return *this;
}

template <class T>
void DynamicArray<T>::shiftLeft(size_t first, size_t last)
{
	assert(first > 0);
	assert(last < size);

	for (size_t i = first - 1; i < last; ++i)
	{
		items[i] = items[i + 1];
	}
}

template <class T>
void DynamicArray<T>::shiftRight(size_t first, size_t last)
{
	assert(last + 1 < size);

	for (size_t i = last + 1; i > first; --i)
	{
		items[i] = items[i - 1];
	}
}

template <class T>
void DynamicArray<T>::extendIfFull()
{
	assert(count <= size);

	if (count >= size)
	{
		resize(size > 0 ? GROWTH_RATE * size : 2);
	}
}

template <class T>
void DynamicArray<T>::setCount(size_t newCount)
{
	assert(newCount <= size);

	count = newCount;
}

template <class T>
inline T* DynamicArray<T>::getItems()
{
	return items;
}

template <class T>
void DynamicArray<T>::resize(size_t newSize)
{
	size_t newCount = (newSize < count) ? newSize : count;
	
	DynamicArray<T> newArray(newSize, newCount);

	for (size_t i = 0; i < newCount; ++i)
	{
		newArray.items[i] = items[i];
	}
	
	swapContentsWith(std::move(newArray));
}

template <class T>
void DynamicArray<T>::copyFrom(const DynamicArray<T>& source)
{
	DynamicArray<T> theCopy(source.size, source.count);

	for (size_t i = 0; i < source.count; ++i)
	{
		theCopy.items[i] = source.items[i];
	}

	swapContentsWith(std::move(theCopy));
}

template <class T>
void DynamicArray<T>::swapContentsWith(DynamicArray<T> temp)
{
	std::swap(count, temp.count);
	std::swap(size, temp.size);
	std::swap(items, temp.items);
}

template <class T>
inline void DynamicArray<T>::destroyItems()
{
	delete[] items;
}

template <class T>
void DynamicArray<T>::nullifyMembers()
{
	items = nullptr;
	count = size = 0;
}
