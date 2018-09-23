
template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
template <class Iterator>
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::PriorityQueue(Iterator& iterator,
																				size_t itemsCount) :
	items(itemsCount, itemsCount)
{
	copyItems(iterator, itemsCount);
	buildHeap();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
template <class Iterator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::copyItems(Iterator& iterator,
																				 size_t itemsCount)
{
	assert(items.getCount() == itemsCount);

	for (size_t i = 0; i < itemsCount; ++i)
	{
		setItemAtWith(i, *iterator);
		++iterator;
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::buildHeap()
{
	for (long nonLeaf = (items.getCount() / 2) - 1; nonLeaf >= 0; --nonLeaf)
	{
		siftDownItemAt(nonLeaf);
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
	std::swap(items, queue.items);
	std::swap(handleUpdator, queue.handleUpdator);
	std::swap(keyAccessor, queue.keyAccessor);
	std::swap(comparator, queue.comparator);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::~PriorityQueue()
{
	invalidateAllHandles();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::invalidateAllHandles()
{
	size_t count = items.getCount();

	for (size_t i = 0; i < count; ++i)
	{
		invalidateHandleAt(i);
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::empty()
{
	invalidateAllHandles();
	items.empty();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::optimiseKey(const PriorityQueueHandle& handleToItem,
																				   const Key& newKey)
{
	verifyHandleValidity(handleToItem);
	setKeyAtWith(handleToItem.index, newKey);
	siftUpItemAt(handleToItem.index);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::verifyHandleValidity(const PriorityQueueHandle& h) const
{
	if (!h.isValid())
	{
		throw std::invalid_argument("Invalid handle!");
	}

	assert(h.index >= 0);
	assert(isWithinHeap(h.index));
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::setKeyAtWith(size_t index, const Key& newKey)
{
	assert(isWithinHeap(index));

	Item& item = items[index];

	if (!comparator(keyAccessor.getKeyOf(item), newKey))
	{
		keyAccessor.setKeyOfWith(item, newKey);
	}
	else
	{
		throw std::invalid_argument("The key can't be worsened!");
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::add(const Item& item)
{	
	items.add(item);
	siftUpItemAt(items.getCount() - 1);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::siftUpItemAt(size_t index)
{
	Item itemToMove = items[index];
	size_t parent;

	while (!isRoot(index))
	{
		parent = computeParentOf(index);

		if (compare(itemToMove, items[parent]))
		{
			setItemAtWith(index, items[parent]);
			index = parent;
		}
		else
		{
			break;
		}
	}

	setItemAtWith(index, itemToMove);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline Item PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::getOptimal() const
{
	verifyQueueIsNotEmpty();

	return items[0];
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

	invalidateHandleAt(0);
	Item optimal = getOptimal();
	moveLastItemAtTopOfHeap();

	if (!isEmpty())
	{
		siftDownItemAt(0);
	}

	return optimal;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::moveLastItemAtTopOfHeap()
{
	assert(!isEmpty());

	size_t indexOfLastItem = items.getCount() - 1;
	setItemAtWith(0, items[indexOfLastItem]);
	items.removeAt(indexOfLastItem);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::siftDownItemAt(size_t index)
{
	Item itemToMove = items[index];
	size_t successor = computeLeftChildOf(index);

	while (isWithinHeap(successor))
	{
		successor = computeOptimalKeySuccessor(successor);

		if (compare(items[successor], itemToMove))
		{
			setItemAtWith(index, items[successor]);
			index = successor;
		}
		else
		{
			break;
		}

		successor = computeLeftChildOf(index);
	}

	setItemAtWith(index, itemToMove);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
size_t PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeOptimalKeySuccessor(size_t leftSuccessor) const
{
	assert(isWithinHeap(leftSuccessor));
	size_t rightSuccessor = leftSuccessor + 1;

	if (isWithinHeap(rightSuccessor)
		&& compare(items[rightSuccessor], items[leftSuccessor]))
	{
		return rightSuccessor;
	}
	else
	{
		return leftSuccessor;
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::setItemAtWith(size_t index,
																							const Item& item)
{
	assert(isWithinHeap(index));

	items[index] = item;
	setHandleAtWith(index, PriorityQueueHandle(index));
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::invalidateHandleAt(size_t index)
{
	setHandleAtWith(index, PriorityQueueHandle(-1));
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::setHandleAtWith(size_t index,
																							  const PriorityQueueHandle& handle)
{
	assert(isWithinHeap(index));

	handleUpdator(items[index], handle);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::compare(const Item& lhs,
																					  const Item& rhs) const
{
	return comparator(keyAccessor.getKeyOf(lhs), keyAccessor.getKeyOf(rhs));
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isEmpty() const
{
	return items.isEmpty();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isWithinHeap(size_t index) const
{
	return index < items.getCount();
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
