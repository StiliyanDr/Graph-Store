
template <class T>
inline LinkedListIterator<T>::LinkedListIterator(Box<T>* current, const LinkedList<T>* owner) :
	current(current), owner(owner)
{
}

template <class T>
LinkedListIterator<T> LinkedListIterator<T>::operator++(int)
{
	LinkedListIterator<T> temp(*this);
	++(*this);

	return temp;
}

template <class T>
LinkedListIterator<T>& LinkedListIterator<T>::operator++()
{
	advance();

	return *this;
}

template <class T>
void LinkedListIterator<T>::advance()
{
	if (current)
	{
		current = current->next;
	}
}

template <class T>
inline T& LinkedListIterator<T>::operator*()
{
	return getCurrentItem();
}

template <class T>
inline T& LinkedListIterator<T>::getCurrentItem()
{
	return current->item;
}

template <class T>
inline bool LinkedListIterator<T>::operator!() const
{
	return !isValid();
}

template <class T>
inline LinkedListIterator<T>::operator bool() const
{
	return isValid();
}

template <class T>
inline bool LinkedListIterator<T>::isValid() const
{
	return current != nullptr;
}

template <class T>
inline bool operator!=(const LinkedListIterator<T>& lhs, const LinkedListIterator<T>& rhs)
{
	return !(lhs == rhs);
}

template <class T>
inline bool operator==(const LinkedListIterator<T>& lhs, const LinkedListIterator<T>& rhs)
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}
