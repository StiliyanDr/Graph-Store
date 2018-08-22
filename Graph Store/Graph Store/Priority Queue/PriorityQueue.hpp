#include <utility>

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::PriorityQueue() :
	itemsCount(0)
{
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::PriorityQueue(Iterator<Item*>& itemsIterator, size_t itemsCount) :
	items(itemsCount, itemsCount)
{
	copyItems(itemsIterator, itemsCount);
	buildHeap();
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::copyItems(Iterator<Item*>& itemsIterator, size_t itemsCount)
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

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::buildHeap()
{
	for (long nonLeaf = (itemsCount / 2) - 1; nonLeaf >= 0; --nonLeaf)
	{
		siftDownItemAt(nonLeaf);
	}
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::PriorityQueue(PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>&& source) :
	items(std::move(source.items)),
	itemsCount(source.itemsCount),
	handleUpdator(std::move(source.handleUpdator)),
	keyAccessor(std::move(source.keyAccessor))
{
	source.itemsCount = 0;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>&
PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::operator=(PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::swapContentsWith(PriorityQueue<Item, Key, HandleUpdator, KeyAccessor> priorityQueue)
{
	std::swap(items, priorityQueue.items);
	std::swap(itemsCount, priorityQueue.itemsCount);
	std::swap(handleUpdator, priorityQueue.handleUpdator);
	std::swap(keyAccessor, priorityQueue.keyAccessor);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::~PriorityQueue()
{
	invalidateAllHandles();
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::invalidateAllHandles()
{
	for (size_t index = 0; index < itemsCount; ++index)
	{
		invalidateHandleAt(index);
	}
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::empty()
{
	invalidateAllHandles();
	items.empty();
	itemsCount = 0;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::decreaseKey(const PriorityQueueHandle& handleToItem,
																	   const Key& newKey)
{
	assert(handleToItem.isValid());
	
	size_t index = handleToItem.index;
	assert(isWithinHeap(index));

	setKeyAtWith(index, newKey);
	siftUpItemAt(index);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::setKeyAtWith(size_t index, const Key& newKey)
{
	Item& item = *items[index];

	assert(!(keyAccessor.getKeyOf(item) < newKey));
	keyAccessor.setKeyOfWith(item, newKey);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::add(Item* item)
{
	assert(item != nullptr);
	
	addAtEnd(item);
	siftUpItemAt(itemsCount - 1);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::addAtEnd(Item* newItem)
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

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::siftUpItemAt(size_t index)
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

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline const Item* PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::getMin() const
{
	assert(!isEmpty());

	return items[0];
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
Item* PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::extractMin()
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

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::siftDownItemAt(size_t index)
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

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
size_t PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::computeMinKeySuccessor(size_t leftSuccessor) const
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

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::setItemAtWith(size_t index, Item* item)
{
	items[index] = item;
	setHandleAtWith(index, PriorityQueueHandle(index));
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::invalidateHandleAt(size_t index)
{
	setHandleAtWith(index, PriorityQueueHandle(-1));
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::setHandleAtWith(size_t index,
																				  const PriorityQueueHandle& handle)
{
	handleUpdator(*items[index], handle);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline bool PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::compare(const Item& lhs, const Item& rhs) const
{
	return keyAccessor.getKeyOf(lhs) < keyAccessor.getKeyOf(rhs);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline bool PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::isEmpty() const
{
	return itemsCount == 0;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline bool PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::isWithinHeap(size_t index) const
{
	return index < itemsCount;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline size_t PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::computeLeftChildOf(size_t index)
{
	return (2 * index) + 1;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline size_t PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::computeParentOf(size_t index)
{
	return (index - 1) / 2;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline bool PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::isRoot(size_t index)
{
	return index == 0;
}
