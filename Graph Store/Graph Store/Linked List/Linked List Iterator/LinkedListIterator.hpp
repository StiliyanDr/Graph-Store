#include <stdexcept>

template <class Item>
inline LinkedListIterator<Item>::LinkedListIterator(Box<Item>* current, const LinkedList<Item>* owner) :
	current(current), owner(owner)
{
}

template <class Item>
LinkedListIterator<Item>& LinkedListIterator<Item>::operator++()
{
	if (!isValid())
	{
		return;
	}

	current = current->next;

	return *this;
}

template <class Item>
inline bool LinkedListIterator<Item>::isValid() const
{
	return current != nullptr;
}

template <class Item>
inline LinkedListIterator<Item>::operator bool() const
{
	return isValid();
}

template <class Item>
inline bool LinkedListIterator<Item>::operator!() const
{
	return !isValid();
}

template <class Item>
inline Item& LinkedListIterator<Item>::operator*()
{
	return getCurrentItem();
}

template <class Item>
inline Item& LinkedListIterator<Item>::getCurrentItem()
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

template <class Item>
inline Item* LinkedListIterator<Item>::operator->()
{
	return &getCurrentItem();
}

template <class Item>
inline bool operator!=(const LinkedListIterator<Item>& lhs, const LinkedListIterator<Item>& rhs)
{
	return !(lhs == rhs);
}

template <class Item>
inline bool operator==(const LinkedListIterator<Item>& lhs, const LinkedListIterator<Item>& rhs)
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}
