#include <stdexcept>
#include <cassert>
#include <utility>


template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs)
{
	DynamicArray<T> temp(lhs);
	temp += rhs;

	return temp;
}


template <class T>
DynamicArray<T> operator+(const T& lhs, const DynamicArray<T>& rhs)
{
	DynamicArray<T> temp(rhs.getCount() + 1);
	temp += lhs;
	temp += rhs;

	return temp;
}


template <class T>
DynamicArray<T> operator+(const DynamicArray<T>& lhs, const T& rhs)
{
	DynamicArray<T> temp(lhs);
	temp += rhs;

	return temp;
}


template <class T>
DynamicArray<T>::DynamicArray(size_t size, size_t count) :
	size(size), items(nullptr)
{
	setCount(count);

	if (size > 0)
		items = new T[size];
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
void DynamicArray<T>::remove(size_t index)
{
	if (index < count)
		shiftLeft(index + 1, --count);
	else
		throw std::out_of_range("Index out of range!");
}


template <class T>
long DynamicArray<T>::search(const T& what) const
{
	for (size_t i = 0; i < count; ++i)
	{
		if (items[i] == what)
			return i;
	}

	return -1;
}


template <class T>
void DynamicArray<T>::addAt(size_t index, const T& item)
{
	if (index <= count)
	{
		extendIfFull();
		shiftRight(index, count - 1);
		items[index] = item;
		++count;
	}
	else
		throw std::out_of_range("Index out of range!");
}


template <class T>
inline void DynamicArray<T>::ensureSize(size_t size)
{
	if (size > this->size)
		resize(size);
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
T& DynamicArray<T>::operator[](size_t index)
{
	if (index < count)
		return items[index];
	else
		throw std::out_of_range("Index out of range!");
}


template <class T>
const T& DynamicArray<T>::operator[](size_t index) const
{
	if (index < count)
		return items[index];
	else
		throw std::out_of_range("Index out of range!");
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
		*this += rhs[i];

	return *this;
}


template <class T>
void DynamicArray<T>::shiftLeft(size_t first, size_t last)
{
	assert(first > 0 && last < size);

	for (size_t i = first - 1; i < last; ++i)
		items[i] = items[i + 1];
}


template <class T>
void DynamicArray<T>::shiftRight(size_t first, size_t last)
{
	assert(last + 1 < size);

	for (size_t i = last + 1; i > first; --i)
		items[i] = items[i - 1];
}


template <class T>
void DynamicArray<T>::extendIfFull()
{
	assert(count <= size);

	if (count >= size)
		resize(size ? 2 * size : 2);
}


template <class T>
void DynamicArray<T>::setCount(size_t newCount)
{
	if (newCount <= size)
		count = newCount;
	else
		throw std::invalid_argument("Count can not be more than size!");
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
	
	DynamicArray<T> temp(newSize, newCount);

	for (size_t i = 0; i < newCount; ++i)
		temp.items[i] = items[i];
	
	swapContentsWith(std::move(temp));
}


template <class T>
void DynamicArray<T>::copyFrom(const DynamicArray<T>& source)
{
	DynamicArray<T> temp(source.size, source.count);

	for (size_t i = 0; i < source.count; ++i)
		temp.items[i] = source.items[i];

	swapContentsWith(std::move(temp));
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
