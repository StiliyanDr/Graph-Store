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
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
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
inline void LinkedList<T>::appendList(const LinkedList<T>& list)
{
	appendChainFrom(list);
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
void LinkedList<T>::insertAfter(LinkedListIterator<T>& iterator, const T& item)
{
	assert(isOwnerOf(iterator));

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
inline bool LinkedList<T>::isOwnerOf(const LinkedListIterator<T>& iterator) const
{
	return this == iterator.owner;
}

template <class T>
void LinkedList<T>::insertBefore(LinkedListIterator<T>& iterator, const T& item)
{
	assert(isOwnerOf(iterator));

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
void LinkedList<T>::addFront(const T &item)
{
	Box<T>* newFirstBox = new Box<T>(item, first);

	if (!first)
	{
		last = newFirstBox;
	}

	first = newFirstBox;

	++size;
}

template <class T>
void LinkedList<T>::addBack(const T &item)
{
	Box<T>* newLastBox = new Box<T>(item);

	if (last)
	{
		last->next = newLastBox;
	}
	else
	{
		first = newLastBox;
	}

	last = newLastBox;

	++size;
}

template <class T>
void LinkedList<T>::removeAt(LinkedListIterator<T>& iterator)
{
	assert(isOwnerOf(iterator));

	if (iterator)
	{
		removeAt(iterator.current);
		iterator.current = nullptr;
	}
}

template <class T>
void LinkedList<T>::removeAfter(LinkedListIterator<T>& iterator)
{
	assert(isOwnerOf(iterator));

	if (iterator && iterator.current->next)
	{
		removeAt(iterator.current->next);
	}
}

template <class T>
void LinkedList<T>::removeBefore(LinkedListIterator<T>& iterator)
{
	assert(isOwnerOf(iterator));

	if (iterator)
	{
		Box<T>* previousBox = findBoxBefore(iterator.current);

		if (previousBox)
		{
			removeAt(previousBox);
		}
	}
}

template <class T>
void LinkedList<T>::removeFirst()
{
	assert(!isEmpty());

	removeAt(first);
}

template <class T>
void LinkedList<T>::removeLast()
{
	assert(!isEmpty());

	removeAt(last);
}

template <class T>
LinkedListIterator<T> LinkedList<T>::search(const T& what)
{
	Box<T>* current = first;

	while (current && current->item != what)
	{
		current = current->next;
	}

	return LinkedListIterator<T>(current, this);
}

template <class T>
inline LinkedListIterator<T> LinkedList<T>::getIteratorToFirst()
{
	return LinkedListIterator<T>(first, this);
}

template <class T>
inline LinkedListIterator<T> LinkedList<T>::getIteratorToLast()
{
	return LinkedListIterator<T>(last, this);
}

template <class T>
void LinkedList<T>::empty()
{
	destroyChain(first);
	nullifyMembers();
}

template <class T>
inline bool LinkedList<T>::isEmpty() const
{
	return size == 0;
}

template <class T>
inline size_t LinkedList<T>::getSize() const
{
	return size;
}

template <class T>
const T& LinkedList<T>::getFirst() const
{
	assert(!isEmpty());

	return first->item;
}

template <class T>
const T& LinkedList<T>::getLast() const
{
	assert(!isEmpty());

	return last->item;
}

template <class T>
void LinkedList<T>::appendChainFrom(LinkedList<T> source)
{
	if (!source.isEmpty())
	{
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
}

template <class T>
void LinkedList<T>::insertAfter(Box<T>* iterator, const T& item)
{
	if (iterator->next)
	{
		iterator->next = new Box<T>(item, iterator->next);
		++size;
	}
	else
	{
		addBack(item);
	}
}

template <class T>
void LinkedList<T>::insertBefore(Box<T>* iterator, const T& item)
{
	Box<T>* previousBox = findBoxBefore(iterator);

	if (previousBox)
	{
		insertAfter(previousBox, item);
	}
	else
	{
		addFront(item);
	}
}

template <class T>
void LinkedList<T>::removeAt(Box<T>* iterator)
{
	Box<T>* previousBox = findBoxBefore(iterator);

	if (previousBox)
	{
		previousBox->next = iterator->next;
	}
	else
	{
		first = iterator->next;
	}

	if (!iterator->next)
	{
		last = previousBox;
	}

	delete iterator;
	--size;
}

template <class T>
Box<T>* LinkedList<T>::findBoxBefore(const Box<T>* box) const
{
	if (box == first)
	{
		return nullptr;
	}

	Box<T>* current = first;

	while (current && current->next != box)
	{
		current = current->next;
	}

	return current;
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
	assert(first == nullptr);

	first = new Box<T>(source.first->item);
	const Box<T>* readFrom = source.first->next;
	Box<T>* writeAfter = first;

	while (readFrom)
	{
		writeAfter->next = new Box<T>(readFrom->item);
		writeAfter = writeAfter->next;
		readFrom = readFrom->next;
	}

	last = writeAfter;
}

template <class T>
void LinkedList<T>::swapContentsWith(LinkedList<T> temp)
{
	std::swap(first, temp.first);
	std::swap(last, temp.last);
	std::swap(size, temp.size);
}

template <class T>
void LinkedList<T>::nullifyMembers()
{
	first = last = nullptr;
	size = 0;
}

template <class T>
Box<T>* LinkedList<T>::findEndOfChain(Box<T>* current)
{
	if (current)
	{
		while (current->next)
		{
			current = current->next;
		}
	}

	return current;
}

template <class T>
void LinkedList<T>::destroyChain(Box<T>* first)
{
	Box<T>* next;

	while (first)
	{
		next = first->next;
		delete first;
		first = next;
	}
}
