#include <assert.h>
#include <utility>

template <class T>
inline LinkedList<T>::LinkedList() noexcept
{
	nullifyMembers();
}

template <class T>
LinkedList<T>::LinkedList(LinkedList&& source) noexcept :
	first(source.first),
	last(source.last),
	size(source.size)
{
	source.nullifyMembers();
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList& source) :
	LinkedList()
{
    if (!source.isEmpty())
    {
        copyChainFrom(source);
        size = source.size;
    }
}

template <class T>
void LinkedList<T>::copyChainFrom(const LinkedList& source)
{
	assert(!source.isEmpty());
	assert(isEmpty());

    first = new Node<T>{ source.first->item };
	auto readFrom = source.first->next;
	auto writeAfter = first;

	while (readFrom != nullptr)
	{
        writeAfter->next = new Node<T>{ readFrom->item };
		writeAfter = writeAfter->next;
		readFrom = readFrom->next;
	}

	last = writeAfter;
}

template <class T>
LinkedList<T>&
LinkedList<T>::operator=(LinkedList<T>&& rhs) noexcept
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class T>
void LinkedList<T>::swapContentsWith(LinkedList<T> list) noexcept
{
	std::swap(first, list.first);
	std::swap(last, list.last);
	std::swap(size, list.size);
}

template <class T>
LinkedList<T>&
LinkedList<T>::operator=(const LinkedList<T>& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(rhs);
	}

	return *this;
}

template <class T>
inline LinkedList<T>::~LinkedList()
{
	destroyChain();
}

template <class T>
void LinkedList<T>::destroyChain() noexcept
{
	while (first != nullptr)
	{
		Node<T>* nextNode = first->next;
		delete first;
		first = nextNode;
	}
}

template <class T>
void LinkedList<T>::empty() noexcept
{
	destroyChain();
	nullifyMembers();
}

template <class T>
void LinkedList<T>::appendList(LinkedList<T> list) noexcept
{
	if (list.isEmpty())
	{
		return;
	}

	if (!this->isEmpty())
	{
		this->last->next = list.first;
	}
	else
	{
		this->first = list.first;
	}

	this->last = list.last;
	this->size += list.size;

	list.nullifyMembers();
}

template <class T>
inline void
LinkedList<T>::insertAfter(const Iterator& iterator,
                           const T& item)
{
    doInsertAfter(iterator, item);
}

template <class T>
inline void
LinkedList<T>::insertAfter(const Iterator& iterator,
                           T&& item)
{
    doInsertAfter(iterator, std::move(item));
}

template <class T>
template <class U>
void LinkedList<T>::doInsertAfter(const Iterator& iterator,
                                  U&& item)
{
	verifyOwnershipOf(iterator);

	if (iterator)
	{
		insertAfter(iterator.current,
                    std::forward<U>(item));
	}
	else
	{
		addBack(std::forward<U>(item));
	}
}

template <class T>
template <class U>
void LinkedList<T>::insertAfter(Node<T>* node,
	                            U&& item)
{
	assert(node != nullptr);

	if (node->next != nullptr)
	{
        node->next = new Node<T>{ std::forward<U>(item),
                                  node->next };
		++size;
	}
	else
	{
		addBack(std::forward<U>(item));
	}
}

template <class T>
inline void LinkedList<T>::addBack(const T& item)
{
    doAddBack(item);
}

template <class T>
inline void LinkedList<T>::addBack(T&& item)
{
    doAddBack(std::move(item));
}

template <class T>
template <class U>
void LinkedList<T>::doAddBack(U&& item)
{
	auto newLastNode =
        new Node<T>{ std::forward<U>(item) };

	if (last != nullptr)
	{
		last->next = newLastNode;
	}
	else
	{
		first = newLastNode;
	}

	last = newLastNode;

	++size;
}

template <class T>
inline void
LinkedList<T>::insertBefore(const Iterator& iterator,
                            const T& item)
{
    doInsertBefore(iterator, item);
}

template <class T>
inline void
LinkedList<T>::insertBefore(const Iterator& iterator,
                            T&& item)
{
    doInsertBefore(iterator, std::move(item));
}

template <class T>
template <class U>
void LinkedList<T>::doInsertBefore(const Iterator& iterator,
                                   U&& item)
{
	verifyOwnershipOf(iterator);

	if (iterator)
	{
		insertBefore(iterator.current,
                     std::forward<U>(item));
	}
	else
	{
		addFront(std::forward<U>(item));
	}
}

template <class T>
template <class U>
void LinkedList<T>::insertBefore(Node<T>* node,
	                             U&& item)
{
	auto previousNode = findNodeBefore(node);

	if (previousNode != nullptr)
	{
		insertAfter(previousNode, std::forward<U>(item));
	}
	else
	{
		addFront(std::forward<U>(item));
	}
}

template <class T>
typename LinkedList<T>::Node<T>*
LinkedList<T>::findNodeBefore(const Node<T>* node) noexcept
{
	assert(node != nullptr);

	if (node == first)
	{
		return nullptr;
	}

	Node<T>* current = first;

	while (current != nullptr && current->next != node)
	{
		current = current->next;
	}

	return current;
}

template <class T>
inline void LinkedList<T>::addFront(const T& item)
{
    doAddFront(item);
}

template <class T>
inline void LinkedList<T>::addFront(T&& item)
{
    doAddFront(std::move(item));
}

template <class T>
template <class U>
void LinkedList<T>::doAddFront(U&& item)
{
	auto newFirstNode =
        new Node<T>{ std::forward<U>(item), first };

	if (first == nullptr)
	{
		last = newFirstNode;
	}

	first = newFirstNode;

	++size;
}

template <class T>
void LinkedList<T>::removeAt(Iterator& iterator)
{
	verifyOwnershipOf(iterator);

	if (iterator)
	{
		removeAt(iterator.current);
		iterator.current = nullptr;
	}
}

template <class T>
void LinkedList<T>::removeAt(Node<T>* node) noexcept
{
	Node<T>* previousNode = findNodeBefore(node);

	if (previousNode != nullptr)
	{
		previousNode->next = node->next;
	}
	else
	{
		first = node->next;
	}

	if (node->next == nullptr)
	{
		last = previousNode;
	}

	delete node;
	--size;
}

template <class T>
void LinkedList<T>::removeAfter(const Iterator& iterator)
{
	verifyOwnershipOf(iterator);

	if (iterator && iterator.current->next != nullptr)
	{
		removeAt(iterator.current->next);
	}
}

template <class T>
void LinkedList<T>::removeBefore(const Iterator& iterator)
{
	verifyOwnershipOf(iterator);

	if (iterator)
	{
		Node<T>* previousNode =
			findNodeBefore(iterator.current);

		if (previousNode != nullptr)
		{
			removeAt(previousNode);
		}
	}
}

template <class T>
void LinkedList<T>::removeFirst()
{
	verifyThatListIsNotEmpty();

	removeAt(first);
}

template <class T>
void LinkedList<T>::removeLast()
{
	verifyThatListIsNotEmpty();

	removeAt(last);
}

template <class T>
inline typename LinkedList<T>::ConstIterator
LinkedList<T>::getConstIterator() const noexcept
{
	return ConstIterator(first, this);
}

template <class T>
inline typename LinkedList<T>::Iterator
LinkedList<T>::getIteratorToFirst() noexcept
{
	return Iterator(first, this);
}

template <class T>
inline typename LinkedList<T>::Iterator
LinkedList<T>::getIteratorToLast() noexcept
{
	return Iterator(last, this);
}

template <class T>
inline bool
LinkedList<T>::isEmpty() const noexcept
{
	return size == 0;
}

template <class T>
inline auto
LinkedList<T>::getSize() const noexcept
-> SizeType
{
	return size;
}

template <class T>
const T& LinkedList<T>::getFirst() const
{
	verifyThatListIsNotEmpty();

	return first->item;
}

template <class T>
const T& LinkedList<T>::getLast() const
{
	verifyThatListIsNotEmpty();

	return last->item;
}

template <class T>
inline void LinkedList<T>::nullifyMembers() noexcept
{
	first = last = nullptr;
	size = 0;
}

template <class T>
inline void
LinkedList<T>::verifyOwnershipOf(const Iterator& iterator) const
{
	if (iterator.owner != this)
	{
		throw std::logic_error("Received iterator from another list!");
	}
}

template <class T>
inline void LinkedList<T>::verifyThatListIsNotEmpty() const
{
	if (isEmpty())
	{
		throw std::logic_error("The list is empty!");
	}
}
