#include <stdexcept>
#include <memory>

template <class T>
template <class Item, bool isConst>
inline LinkedList<T>::LinkedListIterator<Item, isConst>::LinkedListIterator(Node<Item>* current,
																			const LinkedList<Item>* owner) noexcept :
	current(current), owner(owner)
{
}

template <class T>
template <class Item, bool isConst>
inline LinkedList<T>::LinkedListIterator<Item, isConst>::LinkedListIterator(const LinkedListIterator<Item, false>& source) noexcept :
	current(source.current), owner(source.owner)
{
}

template <class T>
template <class Item, bool isConst>
typename LinkedList<T>::LinkedListIterator<Item, isConst>
LinkedList<T>::LinkedListIterator<Item, isConst>::operator++(int) noexcept
{
	LinkedListIterator<Item, isConst> iterator(*this);
	++(*this);

	return iterator;
}

template <class T>
template <class Item, bool isConst>
typename LinkedList<T>::LinkedListIterator<Item, isConst>&
LinkedList<T>::LinkedListIterator<Item, isConst>::operator++() noexcept
{
	if (isValid())
	{
		current = current->next;
	}

	return *this;
}

template <class T>
template <class Item, bool isConst>
inline bool LinkedList<T>::LinkedListIterator<Item, isConst>::isValid() const noexcept
{
	return current != nullptr;
}

template <class T>
template <class Item, bool isConst>
inline LinkedList<T>::LinkedListIterator<Item, isConst>::operator bool() const noexcept
{
	return isValid();
}

template <class T>
template <class Item, bool isConst>
inline bool LinkedList<T>::LinkedListIterator<Item, isConst>::operator!() const noexcept
{
	return !isValid();
}

template <class T>
template <class Item, bool isConst>
inline
typename LinkedList<T>::LinkedListIterator<Item, isConst>::Reference
LinkedList<T>::LinkedListIterator<Item, isConst>::operator*() const
{
	return getCurrentItem();
}

template <class T>
template <class Item, bool isConst>
typename LinkedList<T>::LinkedListIterator<Item, isConst>::Reference
LinkedList<T>::LinkedListIterator<Item, isConst>::getCurrentItem() const
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
template <class Item, bool isConst>
inline
typename LinkedList<T>::LinkedListIterator<Item, isConst>::Pointer
LinkedList<T>::LinkedListIterator<Item, isConst>::operator->() const
{
	return std::addressof(getCurrentItem());
}

template <class Item, bool isConst>
inline bool operator!=(typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
					   typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs) noexcept
{
	return !(lhs == rhs);
}

template <class Item, bool isConst>
inline bool operator==(typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
					   typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs) noexcept
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}
