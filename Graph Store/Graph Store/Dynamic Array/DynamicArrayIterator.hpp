#include <assert.h>

template <class T>
template <class Item, bool isConst>
inline DynamicArray<T>::DynamicArrayIterator<Item, isConst>::DynamicArrayIterator(const DynamicArrayIterator<Item, false>& source) noexcept :
	DynamicArrayIterator(source.currentPosition, source.owner)
{
}

template <class T>
template <class Item, bool isConst>
inline DynamicArray<T>::DynamicArrayIterator<Item, isConst>::DynamicArrayIterator(Position currentPosition,
																				  OwnerPtr owner) noexcept :
	currentPosition(currentPosition),
	owner(owner)
{
}

template <class T>
template <class Item, bool isConst>
auto DynamicArray<T>::DynamicArrayIterator<Item, isConst>::operator++(int) noexcept
-> const DynamicArrayIterator
{
	auto iterator = *this;
	++(*this);

	return iterator;
}

template <class T>
template <class Item, bool isConst>
auto DynamicArray<T>::DynamicArrayIterator<Item, isConst>::operator++() noexcept
-> DynamicArrayIterator&
{
	if (isValid())
	{
		++currentPosition;
	}

	return *this;
}

template <class T>
template <class Item, bool isConst>
inline bool
DynamicArray<T>::DynamicArrayIterator<Item, isConst>::isValid() const noexcept
{
	return currentPosition < owner->getCount();
}

template <class T>
template <class Item, bool isConst>
auto DynamicArray<T>::DynamicArrayIterator<Item, isConst>::getCurrentItem() const
-> Reference
{
	assert(isValid());

	return (*owner)[currentPosition];
}

template <class Item, bool isConst>
inline bool operator!=(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
					   typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs) noexcept
{
	return !(lhs == rhs);
}

template <class Item, bool isConst>
inline bool operator==(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
					   typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs) noexcept
{
	return lhs.owner == rhs.owner && lhs.currentPosition == rhs.currentPosition;
}
