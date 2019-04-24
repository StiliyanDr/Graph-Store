#include <assert.h>
#include <utility>
#include <stdexcept>

template <class T>
inline DynamicArray<T>::DynamicArray() noexcept :
	DynamicArray(0, 0)
{
}

template <class T>
DynamicArray<T>::DynamicArray(std::size_t size, std::size_t count) :
	size(size), items(nullptr)
{
	setCount(count);

	if (size > 0)
	{
		items = new T[size];
	}
}

template <class T>
void DynamicArray<T>::setCount(std::size_t newCount)
{
	if (newCount <= size)
	{
		count = newCount;
	}
	else
	{
		throw std::invalid_argument("Count must not exceed size!");
	}
}

template <class T>
inline DynamicArray<T>::DynamicArray(DynamicArray<T>&& source) noexcept :
	count(source.count), size(source.size), items(source.items)
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
	DynamicArray<T> theCopy(source.size, source.count);

	for (std::size_t i = 0; i < source.count; ++i)
	{
		theCopy.items[i] = source.items[i];
	}

	swapContentsWith(std::move(theCopy));
}

template <class T>
void DynamicArray<T>::swapContentsWith(DynamicArray<T> other)
{
	std::swap(count, other.count);
	std::swap(size, other.size);
	std::swap(items, other.items);
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& rhs) noexcept
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
DynamicArray<T> operator+(const DynamicArray<T>& lhs,
						  const DynamicArray<T>& rhs)
{
	DynamicArray<T> newArray(lhs);
	newArray += rhs;

	return newArray;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator+=(const DynamicArray<T>& rhs)
{
	for (std::size_t i = 0; i < rhs.count; ++i)
	{
		*this += rhs[i];
	}

	return *this;
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
inline DynamicArray<T>& DynamicArray<T>::operator+=(const T& item)
{
	add(item);

	return *this;
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

	items[count] = std::forward<U>(item);
	++count;
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
void DynamicArray<T>::resize(std::size_t newSize)
{
	std::size_t newCount = (newSize < count) ? newSize : count;

	DynamicArray<T> newArray(newSize, newCount);

	for (std::size_t i = 0; i < newCount; ++i)
	{
		newArray.items[i] = moveAssignIfNoexcept(items[i]);
	}

	swapContentsWith(std::move(newArray));
}

template <class T>
using ConditionalReference = typename std::conditional<
	!std::is_nothrow_move_assignable<T>::value && std::is_copy_assignable<T>::value,
	const T&,
	T&&
>::type;

template <class T>
inline constexpr ConditionalReference<T>
moveAssignIfNoexcept(T& object) noexcept
{
	return static_cast<ConditionalReference<T>>(object);
}

template <class T>
inline void DynamicArray<T>::addAt(std::size_t index, const T& item)
{
	doAddAt(index, item);
}

template <class T>
inline void DynamicArray<T>::addAt(std::size_t index, T&& item)
{
	doAddAt(index, std::move(item));
}

template <class T>
template <class U>
void DynamicArray<T>::doAddAt(std::size_t index, U&& item)
{
	if (index <= count)
	{
		extendIfFull();
		shiftRight(index, count - 1);
		items[index] = std::forward<U>(item);
		++count;
	}
	else
	{
		throw std::out_of_range("Index out of range!");
	}
}

template <class T>
void DynamicArray<T>::shiftRight(std::size_t first, std::size_t last)
{
	assert(last + 1 < size);

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
		removeAt(count - 1);
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

	shiftLeft(index + 1, --count);
}

template <class T>
void DynamicArray<T>::shiftLeft(std::size_t first, std::size_t last)
{
	assert(first > 0);
	assert(last < size);

	for (std::size_t i = first - 1; i < last; ++i)
	{
		items[i] = std::move(items[i + 1]);
	}
}

template <class T>
inline T& DynamicArray<T>::operator[](std::size_t index)
{
	validateIndex(index);

	return items[index];
}

template <class T>
inline const T& DynamicArray<T>::operator[](std::size_t index) const
{
	validateIndex(index);

	return items[index];
}

template <class T>
inline void DynamicArray<T>::ensureSize(std::size_t size)
{
	if (size > this->size)
	{
		resize(size);
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
	count = size = 0;
}

template <class T>
inline void DynamicArray<T>::validateIndex(std::size_t i) const
{
	if (i >= count)
	{
		throw std::out_of_range("Index out of range!");
	}
}

template <class T>
inline bool DynamicArray<T>::isEmpty() const noexcept
{
	return count == 0;
}

template <class T>
inline std::size_t DynamicArray<T>::getSize() const noexcept
{
	return size;
}

template <class T>
inline std::size_t DynamicArray<T>::getCount() const noexcept
{
	return count;
}

template <class T>
inline void DynamicArray<T>::destroyItems()
{
	delete[] items;
}
