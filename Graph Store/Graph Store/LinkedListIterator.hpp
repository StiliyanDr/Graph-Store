
template <class T>
inline LinkedListIterator<T>::LinkedListIterator(Box<T>* current, const LinkedList<T>* owner) :
	current(current), owner(owner)
{
}


template <class T>
LinkedListIterator<T>& LinkedListIterator<T>::operator++()
{
	if (current)
		current = current->next;

	return *this;
}


template <class T>
LinkedListIterator<T> LinkedListIterator<T>::operator++(int)
{
	LinkedListIterator<T> temp(*this);
	++(*this);

	return temp;
}


template <class T>
inline T& LinkedListIterator<T>::operator*()
{
	return current->item;
}


template <class T>
inline bool LinkedListIterator<T>::operator!() const
{
	return !current;
}


template <class T>
inline LinkedListIterator<T>::operator bool() const
{
	return current != nullptr;
}


template <class T>
inline bool operator==(const LinkedListIterator<T>& lhs, const LinkedListIterator<T>& rhs)
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}


template <class T>
inline bool operator!=(const LinkedListIterator<T>& lhs, const LinkedListIterator<T>& rhs)
{
	return !(lhs == rhs);
}
