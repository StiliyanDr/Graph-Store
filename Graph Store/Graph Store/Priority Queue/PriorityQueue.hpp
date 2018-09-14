#include <utility>

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::PriorityQueue() :
	itemsCount(0)
{
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::PriorityQueue(Iterator<Item*>& itemsIterator, size_t itemsCount) :
	items(itemsCount, itemsCount)
{
	copyItems(itemsIterator, itemsCount);
	buildHeap();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::copyItems(Iterator<Item*>& itemsIterator, size_t itemsCount)
{
	assert(items.getCount() == itemsCount);
	Item* item;

	for (size_t index = 0; index < itemsCount; ++index)
	{
		assert(itemsIterator);
		item = *itemsIterator;

		assert(item != nullptr);
		setItemAtWith(index, item);

		++itemsIterator;
	}

	this->itemsCount = itemsCount;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::buildHeap()
{
	for (long nonLeaf = (itemsCount / 2) - 1; nonLeaf >= 0; --nonLeaf)
	{
		siftDownItemAt(nonLeaf);
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::PriorityQueue(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&& source) :
	items(std::move(source.items)),
	itemsCount(source.itemsCount),
	handleUpdator(std::move(source.handleUpdator)),
	keyAccessor(std::move(source.keyAccessor)),
	comparator(std::move(source.comparator))
{
	source.itemsCount = 0;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::operator=(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::swapContentsWith(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator> priorityQueue)
{
	std::swap(items, priorityQueue.items);
	std::swap(itemsCount, priorityQueue.itemsCount);
	std::swap(handleUpdator, priorityQueue.handleUpdator);
	std::swap(keyAccessor, priorityQueue.keyAccessor);
	std::swap(comparator, priorityQueue.comparator);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::~PriorityQueue()
{
	invalidateAllHandles();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::invalidateAllHandles()
{
	for (size_t index = 0; index < itemsCount; ++index)
	{
		invalidateHandleAt(index);
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::empty()
{
	invalidateAllHandles();
	items.empty();
	itemsCount = 0;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::decreaseKey(const PriorityQueueHandle& handleToItem,
																	   const Key& newKey)
{
	assert(handleToItem.isValid());
	
	size_t index = handleToItem.index;
	assert(isWithinHeap(index));

	setKeyAtWith(index, newKey);
	siftUpItemAt(index);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::setKeyAtWith(size_t index, const Key& newKey)
{
	Item& item = *items[index];

	assert(!comparator(keyAccessor.getKeyOf(item), newKey));
	keyAccessor.setKeyOfWith(item, newKey);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::add(Item* item)
{
	assert(item != nullptr);
	
	addAtEnd(item);
	siftUpItemAt(itemsCount - 1);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::addAtEnd(Item* newItem)
{
	if (itemsCount < items.getCount())
	{
		items[itemsCount] = newItem;
	}
	else
	{
		items.add(newItem);
	}

	++itemsCount;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::siftUpItemAt(size_t index)
{
	Item* itemToMove = items[index];
	size_t parent;

	while (!isRoot(index))
	{
		parent = computeParentOf(index);

		if (compare(*itemToMove, *items[parent]))
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
inline const Item* PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::getMin() const
{
	assert(!isEmpty());

	return items[0];
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
Item* PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::extractMin()
{
	assert(!isEmpty());

	invalidateHandleAt(0);

	Item* min = items[0];
	--itemsCount;

	if (!isEmpty())
	{
		setItemAtWith(0, items[itemsCount]);
		siftDownItemAt(0);
	}

	return min;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::siftDownItemAt(size_t index)
{
	Item* itemToMove = items[index];
	size_t successor = computeLeftChildOf(index);

	while (isWithinHeap(successor))
	{
		successor = computeMinKeySuccessor(successor);

		if (compare(*items[successor], *itemToMove))
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
size_t PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeMinKeySuccessor(size_t leftSuccessor) const
{
	assert(isWithinHeap(leftSuccessor));
	size_t rightSuccessor = leftSuccessor + 1;

	if (isWithinHeap(rightSuccessor)
		&& compare(*items[rightSuccessor], *items[leftSuccessor]))
	{
		return rightSuccessor;
	}
	else
	{
		return leftSuccessor;
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::setItemAtWith(size_t index, Item* item)
{
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
	handleUpdator(*items[index], handle);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::compare(const Item& lhs, const Item& rhs) const
{
	return comparator(keyAccessor.getKeyOf(lhs), keyAccessor.getKeyOf(rhs));
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isEmpty() const
{
	return itemsCount == 0;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isWithinHeap(size_t index) const
{
	return index < itemsCount;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline size_t PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeLeftChildOf(size_t index)
{
	return (2 * index) + 1;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline size_t PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeParentOf(size_t index)
{
	return (index - 1) / 2;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isRoot(size_t index)
{
	return index == 0;
}
