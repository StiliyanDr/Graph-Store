#include <cassert>
#include <stdexcept>


template <class T>
DynamicArrayIterator<T>& DynamicArrayIterator<T>::operator++()
{
	if (isValid())
	{
		assert(current >= 0);
		assert(current < owner->getCount());

		++current;

		if (current >= owner->getCount())
			current = INVALID_POSITION;
	}
}


template <class T>
DynamicArrayIterator<T> DynamicArrayIterator<T>::operator++(int)
{
	DynamicArrayIterator<T> temp(*this);
	++(*this);

	return temp;
}


template <class T>
T& DynamicArrayIterator<T>::operator*()
{
	if (isValid())
		return (*owner)[current];
	else
		throw std::out_of_range("Iterator out of range!");
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
inline DynamicArrayIterator<T>::DynamicArrayIterator(Position current, DynamicArray<T>* owner) :
	current(current), owner(owner)
{
}


template <class T>
inline bool DynamicArrayIterator<T>::isValid() const
{
	return current != INVALID_POSITION;
}


template <class T>
inline bool operator==(const DynamicArrayIterator<T>& lhs, const DynamicArrayIterator<T>& rhs)
{
	return lhs.owner == rhs.owner && lhs.current == rhs.current;
}


template <class T>
inline bool operator!=(const DynamicArrayIterator<T>& lhs, const DynamicArrayIterator<T>& rhs)
{
	return !(lhs == rhs);
}
