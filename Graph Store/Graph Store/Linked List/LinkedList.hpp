#include <assert.h>
#include <utility>

template <class T>
inline LinkedList<T>::LinkedList()
{
	nullifyMembers();
}

template <class T>
LinkedList<T>::LinkedList(LinkedList<T>&& source) :
	first(source.first), last(source.last), size(source.size)
{
	source.nullifyMembers();
}

template <class T>
inline LinkedList<T>::LinkedList(const LinkedList<T>& source)
{
	copyFrom(source);
}

template <class T>
void LinkedList<T>::copyFrom(const LinkedList<T>& source)
{
	nullifyMembers();

	if (!source.isEmpty())
	{
		try
		{
			copyChainFrom(source);
		}
		catch (std::bad_alloc&)
		{
			destroyChain(first);
			throw;
		}

		size = source.size;
	}
}

template <class T>
void LinkedList<T>::copyChainFrom(const LinkedList<T>& source)
{
	assert(!source.isEmpty());
	assert(isEmpty());

	first = new Node<T>(source.first->item);
	const Node<T>* readFrom = source.first->next;
	Node<T>* writeAfter = first;

	while (readFrom)
	{
		writeAfter->next = new Node<T>(readFrom->item);
		writeAfter = writeAfter->next;
		readFrom = readFrom->next;
	}

	last = writeAfter;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class T>
void LinkedList<T>::swapContentsWith(LinkedList<T> list)
{
	std::swap(first, list.first);
	std::swap(last, list.last);
	std::swap(size, list.size);
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs)
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
	destroyChain(first);
}

template <class T>
void LinkedList<T>::destroyChain(Node<T>* firstNode)
{
	Node<T>* nodeToDestroy = firstNode;
	Node<T>* nextNode;

	while (nodeToDestroy != nullptr)
	{
		nextNode = nodeToDestroy->next;
		delete nodeToDestroy;
		nodeToDestroy = nextNode;
	}
}

template <class T>
void LinkedList<T>::empty()
{
	destroyChain(first);
	nullifyMembers();
}

template <class T>
inline void LinkedList<T>::appendList(const LinkedList<T>& list)
{
	appendChainFrom(list);
}

template <class T>
void LinkedList<T>::appendChainFrom(LinkedList<T> source)
{
	if (source.isEmpty())
	{
		return;
	}

	if (!this->isEmpty())
	{
		this->last->next = source.first;
	}
	else
	{
		this->first = source.first;
	}

	this->last = source.last;
	this->size += source.size;

	source.nullifyMembers();
}

template <class T>
void LinkedList<T>::appendList(LinkedList<T>&& list)
{
	if (this != &list)
	{
		appendChainFrom(std::move(list));
	}
}

template <class T>
void LinkedList<T>::insertAfter(const Iterator& iterator, const T& item)
{
	verifyOwnershipOf(iterator);

	if (iterator)
	{
		insertAfter(iterator.current, item);
	}
	else
	{
		addBack(item);
	}
}

template <class T>
void LinkedList<T>::insertAfter(Node<T>* node, const T& item)
{
	assert(node != nullptr);

	if (node->next != nullptr)
	{
		node->next = new Node<T>(item, node->next);
		++size;
	}
	else
	{
		addBack(item);
	}
}

template <class T>
void LinkedList<T>::addBack(const T &item)
{
	Node<T>* newLastNode = new Node<T>(item);

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
void LinkedList<T>::insertBefore(const Iterator& iterator, const T& item)
{
	verifyOwnershipOf(iterator);

	if (iterator)
	{
		insertBefore(iterator.current, item);
	}
	else
	{
		addFront(item);
	}
}

template <class T>
void LinkedList<T>::insertBefore(Node<T>* node, const T& item)
{
	Node<T>* previousNode = findNodeBefore(node);

	if (previousNode != nullptr)
	{
		insertAfter(previousNode, item);
	}
	else
	{
		addFront(item);
	}
}

template <class T>
typename LinkedList<T>::Node<T>*
LinkedList<T>::findNodeBefore(const Node<T>* node) const
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
void LinkedList<T>::addFront(const T &item)
{
	Node<T>* newFirstNode = new Node<T>(item, first);

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
void LinkedList<T>::removeAt(Node<T>* node)
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
		Node<T>* previousNode = findNodeBefore(iterator.current);

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
LinkedList<T>::getConstIterator() const
{
	return ConstIterator(first, this);
}

template <class T>
inline typename LinkedList<T>::Iterator
LinkedList<T>::getIteratorToFirst()
{
	return Iterator(first, this);
}

template <class T>
inline typename LinkedList<T>::Iterator
LinkedList<T>::getIteratorToLast()
{
	return Iterator(last, this);
}

template <class T>
inline bool LinkedList<T>::isEmpty() const
{
	return size == 0;
}

template <class T>
inline std::size_t LinkedList<T>::getSize() const
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
inline void LinkedList<T>::nullifyMembers()
{
	first = last = nullptr;
	size = 0;
}

template <class T>
inline void LinkedList<T>::verifyOwnershipOf(const Iterator& iterator) const
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
