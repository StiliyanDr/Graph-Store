#include <stdexcept>

template <class T>
template <class Item>
inline LinkedList<T>::LinkedListIterator<Item>::LinkedListIterator(Box<Item>* current,
																   const LinkedList<Item>* owner) :
	current(current), owner(owner)
{
}

template <class T>
template <class Item>
typename LinkedList<T>::LinkedListIterator<Item>&
LinkedList<T>::LinkedListIterator<Item>::operator++()
{
	if (!isValid())
	{
		return;
	}

	current = current->next;

	return *this;
}

template <class T>
template <class Item>
inline bool LinkedList<T>::LinkedListIterator<Item>::isValid() const
{
	return current != nullptr;
}

template <class T>
template <class Item>
inline LinkedList<T>::LinkedListIterator<Item>::operator bool() const
{
	return isValid();
}

template <class T>
template <class Item>
inline bool LinkedList<T>::LinkedListIterator<Item>::operator!() const
{
	return !isValid();
}

template <class T>
template <class Item>
inline Item& LinkedList<T>::LinkedListIterator<Item>::operator*()
{
	return getCurrentItem();
}

template <class T>
template <class Item>
Item& LinkedList<T>::LinkedListIterator<Item>::getCurrentItem()
{
	if (isValid())
	{
		return current->item;
	}
	else
	{
		throw std::out_of_range("Iterator out of range!");
	}
}

template <class T>
template <class Item>
inline Item* LinkedList<T>::LinkedListIterator<Item>::operator->()
{
	return &getCurrentItem();
}

template <class Item>
inline bool operator!=(typename const LinkedList<Item>::LinkedListIterator<Item>& lhs,
					   typename const LinkedList<Item>::LinkedListIterator<Item>& rhs)
{
	return !(lhs == rhs);
}

template <class Item>
inline bool operator==(typename const LinkedList<Item>::LinkedListIterator<Item>& lhs,
					   typename const LinkedList<Item>::LinkedListIterator<Item>& rhs)
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}
