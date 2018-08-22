#include <stdexcept>

template <class T>
template <class Item>
inline DynamicArray<T>::DynamicArrayIterator<Item>::DynamicArrayIterator(Position currentPosition,
																		 DynamicArray<Item>* owner) :
	currentPosition(currentPosition), owner(owner)
{
}

template <class T>
template <class Item>
typename DynamicArray<T>::DynamicArrayIterator<Item>&
DynamicArray<T>::DynamicArrayIterator<Item>::operator++()
{
	if (isValid())
	{
		++currentPosition;
	}

	return *this;
}

template <class T>
template <class Item>
inline bool DynamicArray<T>::DynamicArrayIterator<Item>::isValid() const
{
	return currentPosition < owner->getCount();
}

template <class T>
template <class Item>
inline DynamicArray<T>::DynamicArrayIterator<Item>::operator bool() const
{
	return isValid();
}

template <class T>
template <class Item>
inline bool DynamicArray<T>::DynamicArrayIterator<Item>::operator!() const
{
	return !isValid();
}

template <class T>
template <class Item>
inline Item& DynamicArray<T>::DynamicArrayIterator<Item>::operator*()
{
	return getCurrentItem();
}

template <class T>
template <class Item>
Item& DynamicArray<T>::DynamicArrayIterator<Item>::getCurrentItem()
{
	if (isValid())
	{
		return (*owner)[currentPosition];
	}
	else
	{
		throw std::out_of_range("Iterator out of range!");
	}
}

template <class T>
template <class Item>
inline Item* DynamicArray<T>::DynamicArrayIterator<Item>::operator->()
{
	return &getCurrentItem();
}

template <class Item>
inline bool operator!=(typename const DynamicArray<Item>::DynamicArrayIterator<Item>& lhs,
					   typename const DynamicArray<Item>::DynamicArrayIterator<Item>& rhs)
{
	return !(lhs == rhs);
}

template <class Item>
inline bool operator==(typename const DynamicArray<Item>::DynamicArrayIterator<Item>& lhs,
					   typename const DynamicArray<Item>::DynamicArrayIterator<Item>& rhs)
{
	return lhs.owner == rhs.owner && lhs.currentPosition == rhs.currentPosition;
}
