
template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
HandleUpdator PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::handleUpdator;

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
KeyAccessor PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::keyAccessor;

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
Comparator PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::comparator;

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::Element(const Item& item) :
	item(item)
{
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::compare(const Element& lhs,
																							   const Element& rhs)
{
	return comparator(lhs.getKey(), rhs.getKey());
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline const Key& PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::getKey() const
{
	return keyAccessor.getKeyOf(item);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::optimiseKey(const Key& newKey)
{
	if (!comparator(getKey(), newKey))
	{
		keyAccessor.setKeyOfWith(item, newKey);
	}
	else
	{
		throw std::invalid_argument("The key can't be worsened!");
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::invalidateHandle()
{
	setHandle(Handle());
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::setHandle(const Handle& handle)
{
	handleUpdator(item, handle);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline const Item& PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::getItem() const
{
	return item;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
template <class Iterator>
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::PriorityQueue(Iterator& iterator,
																				size_t itemsCount) :
	elements(itemsCount, itemsCount)
{
	copyItems(iterator, itemsCount);
	buildHeap();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
template <class Iterator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::copyItems(Iterator& iterator,
																				 size_t itemsCount)
{
	assert(elements.getCount() == itemsCount);

	for (size_t i = 0; i < itemsCount; ++i)
	{
		setElementAtWith(i, Element(*iterator));
		++iterator;
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::buildHeap()
{
	for (long nonLeaf = (elements.getCount() / 2) - 1; nonLeaf >= 0; --nonLeaf)
	{
		siftDownElementAt(nonLeaf);
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::operator=(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&& rhs)
{
	if (this != &rhs)
	{
		invalidateAllHandles();
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::swapContentsWith(
	PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator> queue)
{
	std::swap(elements, queue.elements);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::~PriorityQueue()
{
	invalidateAllHandles();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::invalidateAllHandles()
{
	size_t count = elements.getCount();

	for (size_t i = 0; i < count; ++i)
	{
		elements[i].invalidateHandle();
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::empty()
{
	invalidateAllHandles();
	elements.empty();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::optimiseKey(const Handle& handle,
																				   const Key& newKey)
{
	verifyHandleValidity(handle);

	elements[handle.index].optimiseKey(newKey);
	siftUpElementAt(handle.index);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::verifyHandleValidity(const Handle& h) const
{
	if (!h.isValid())
	{
		throw std::invalid_argument("Invalid handle!");
	}

	assert(h.index >= 0);
	assert(isWithinHeap(h.index));
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::add(const Item& item)
{	
	elements.add(Element(item));
	siftUpElementAt(elements.getCount() - 1);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::siftUpElementAt(size_t index)
{
	Element elementToMove = elements[index];
	size_t parent;

	while (!isRoot(index))
	{
		parent = computeParentOf(index);

		if (Element::compare(elementToMove, elements[parent]))
		{
			setElementAtWith(index, elements[parent]);
			index = parent;
		}
		else
		{
			break;
		}
	}

	setElementAtWith(index, elementToMove);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline Item PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::getOptimal() const
{
	verifyQueueIsNotEmpty();

	return elements[0].getItem();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::verifyQueueIsNotEmpty() const
{
	if (isEmpty())
	{
		throw std::logic_error("The queue is empty!");
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
Item PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::extractOptimal()
{
	verifyQueueIsNotEmpty();

	elements[0].invalidateHandle();
	Item optimal = getOptimal();
	moveLastElementAtTopOfHeap();

	if (!isEmpty())
	{
		siftDownElementAt(0);
	}

	return optimal;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::moveLastElementAtTopOfHeap()
{
	assert(!isEmpty());

	size_t indexOfLastElement = elements.getCount() - 1;
	setElementAtWith(0, elements[indexOfLastElement]);
	elements.removeAt(indexOfLastElement);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::siftDownElementAt(size_t index)
{
	Element elementToMove = elements[index];
	size_t successor = computeLeftChildOf(index);

	while (isWithinHeap(successor))
	{
		successor = computeOptimalKeySuccessor(successor);

		if (Element::compare(elements[successor], elementToMove))
		{
			setElementAtWith(index, elements[successor]);
			index = successor;
		}
		else
		{
			break;
		}

		successor = computeLeftChildOf(index);
	}

	setElementAtWith(index, elementToMove);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
size_t PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeOptimalKeySuccessor(size_t leftSuccessor) const
{
	assert(isWithinHeap(leftSuccessor));
	size_t rightSuccessor = leftSuccessor + 1;

	if (isWithinHeap(rightSuccessor)
		&& Element::compare(elements[rightSuccessor], elements[leftSuccessor]))
	{
		return rightSuccessor;
	}
	else
	{
		return leftSuccessor;
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::setElementAtWith(size_t index,
																							   const Element& element)
{
	assert(isWithinHeap(index));

	elements[index] = element;
	elements[index].setHandle(Handle(index));
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isEmpty() const
{
	return elements.isEmpty();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isWithinHeap(size_t index) const
{
	return index < elements.getCount();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline size_t PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeLeftChildOf(size_t index)
{
	return (2 * index) + 1;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline size_t PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeParentOf(size_t index)
{
	assert(index > 0);

	return (index - 1) / 2;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isRoot(size_t index)
{
	return index == 0;
}
