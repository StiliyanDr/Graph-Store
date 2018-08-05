#include <assert.h>

template <class T>
inline DynamicArrayIterator<T>::DynamicArrayIterator(Position currentPosition, DynamicArray<T>* owner) :
	currentPosition(currentPosition), owner(owner)
{
}

template <class T>
DynamicArrayIterator<T> DynamicArrayIterator<T>::operator++(int)
{
	DynamicArrayIterator<T> iterator(*this);
	++(*this);

	return iterator;
}

template <class T>
DynamicArrayIterator<T>& DynamicArrayIterator<T>::operator++()
{
	advance();

	return *this;
}

template <class T>
void DynamicArrayIterator<T>::advance()
{
	if (!isValid())
	{
		return;
	}

	assert(currentPosition >= 0);
	assert(currentPosition < owner->getCount());

	++currentPosition;

	if (currentPosition >= owner->getCount())
	{
		currentPosition = INVALID_POSITION;
	}
}

template <class T>
inline bool DynamicArrayIterator<T>::operator!() const
{
	return !isValid();
}

template <class T>
inline DynamicArrayIterator<T>::operator bool() const
{
	return isValid();
}

template <class T>
inline bool DynamicArrayIterator<T>::isValid() const
{
	return currentPosition != INVALID_POSITION;
}

template <class T>
inline T& DynamicArrayIterator<T>::operator*()
{
	return getCurrentItem();
}

template <class T>
T& DynamicArrayIterator<T>::getCurrentItem()
{
	assert(isValid());

	return (*owner)[currentPosition];
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
