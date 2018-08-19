#include <stdexcept>

template <class T>
inline DynamicArrayIterator<T>::DynamicArrayIterator(Position currentPosition, DynamicArray<T>* owner) :
	currentPosition(currentPosition), owner(owner)
{
}

template <class T>
DynamicArrayIterator<T>& DynamicArrayIterator<T>::operator++()
{
	if (!isValid())
	{
		return;
	}

	++currentPosition;

	return *this;
}

template <class T>
inline bool DynamicArrayIterator<T>::isValid() const
{
	return currentPosition < owner->getCount();
}

template <class T>
inline DynamicArrayIterator<T>::operator bool() const
{
	return isValid();
}

template <class T>
inline bool DynamicArrayIterator<T>::operator!() const
{
	return !isValid();
}

template <class T>
inline T& DynamicArrayIterator<T>::operator*()
{
	return getCurrentItem();
}

template <class T>
T& DynamicArrayIterator<T>::getCurrentItem()
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
inline T* DynamicArrayIterator<T>::operator->()
{
	return &getCurrentItem();
}

template <class T>
inline bool operator!=(const DynamicArrayIterator<T>& lhs, const DynamicArrayIterator<T>& rhs)
{
	return !(lhs == rhs);
}

template <class T>
inline bool operator==(const DynamicArrayIterator<T>& lhs, const DynamicArrayIterator<T>& rhs)
{
	return lhs.owner == rhs.owner && lhs.currentPosition == rhs.currentPosition;
}
