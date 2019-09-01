#include <assert.h>

template <class T>
template <class Item, bool isConst>
inline
LinkedList<T>::LinkedListIterator<Item, isConst>::LinkedListIterator(
    Node<Item>* current,
    const LinkedList<Item>* owner
) noexcept :
	current(current),
    owner(owner)
{
    assert(owner != nullptr);
}

template <class T>
template <class Item, bool isConst>
LinkedList<T>::LinkedListIterator<Item, isConst>::LinkedListIterator(
    const LinkedListIterator<Item, false>& source
) noexcept :
    LinkedListIterator{ source.current, source.owner }
{
}

template <class T>
template <class Item, bool isConst>
auto LinkedList<T>::LinkedListIterator<Item, isConst>::operator++(int) noexcept
-> LinkedListIterator
{
	auto copy = *this;
	++(*this);

	return copy;
}

template <class T>
template <class Item, bool isConst>
auto LinkedList<T>::LinkedListIterator<Item, isConst>::operator++() noexcept
-> LinkedListIterator&
{
	if (isValid())
	{
		current = current->next;
	}

	return *this;
}

template <class T>
template <class Item, bool isConst>
inline bool LinkedList<T>::LinkedListIterator<Item, isConst>::isValid()
const noexcept
{
	return current != nullptr;
}

template <class T>
template <class Item, bool isConst>
auto LinkedList<T>::LinkedListIterator<Item, isConst>::getCurrentItem()
const noexcept -> Reference
{
    assert(isValid());

    return current->item;
}

template <class Item, bool isConst>
inline bool operator!=(
    typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
    typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs
) noexcept
{
	return !(lhs == rhs);
}

template <class Item, bool isConst>
inline bool operator==(
    typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
    typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs
) noexcept
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}
