#include <stdexcept>

template <class T>
template <class Item, bool isConst>
inline DynamicArray<T>::DynamicArrayIterator<Item, isConst>::DynamicArrayIterator(Position currentPosition,
																				  DynamicArray<Item>* owner) :
	currentPosition(currentPosition), owner(owner)
{
}

template <class T>
template <class Item, bool isConst>
typename DynamicArray<T>::DynamicArrayIterator<Item, isConst>&
DynamicArray<T>::DynamicArrayIterator<Item, isConst>::operator++()
{
	if (isValid())
	{
		++currentPosition;
	}

	return *this;
}

template <class T>
template <class Item, bool isConst>
inline bool DynamicArray<T>::DynamicArrayIterator<Item, isConst>::isValid() const
{
	return currentPosition < owner->getCount();
}

template <class T>
template <class Item, bool isConst>
inline DynamicArray<T>::DynamicArrayIterator<Item, isConst>::operator bool() const
{
	return isValid();
}

template <class T>
template <class Item, bool isConst>
inline bool DynamicArray<T>::DynamicArrayIterator<Item, isConst>::operator!() const
{
	return !isValid();
}

template <class T>
template <class Item, bool isConst>
inline typename DynamicArray<T>::DynamicArrayIterator<Item, isConst>::Reference
DynamicArray<T>::DynamicArrayIterator<Item, isConst>::operator*() const
{
	return getCurrentItem();
}

template <class T>
template <class Item, bool isConst>
typename DynamicArray<T>::DynamicArrayIterator<Item, isConst>::Reference
DynamicArray<T>::DynamicArrayIterator<Item, isConst>::getCurrentItem() const
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
template <class Item, bool isConst>
inline typename DynamicArray<T>::DynamicArrayIterator<Item, isConst>::Pointer
DynamicArray<T>::DynamicArrayIterator<Item, isConst>::operator->() const
{
	return &getCurrentItem();
}

template <class Item, bool isConst>
inline bool operator!=(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
					   typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs)
{
	return !(lhs == rhs);
}

template <class Item, bool isConst>
inline bool operator==(typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& lhs,
					   typename const DynamicArray<Item>::DynamicArrayIterator<Item, isConst>& rhs)
{
	return lhs.owner == rhs.owner && lhs.currentPosition == rhs.currentPosition;
}
