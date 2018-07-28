#include <utility>

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::PriorityQueue() :
	elementsCount(0)
{
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::PriorityQueue(Iterator<Element<Item, Key>>& elementsIterator,
																	size_t elementsCount) :
	elements(elementsCount, elementsCount)
{
	copyElements(elementsIterator, elementsCount);
	buildHeap();
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::copyElements(Iterator<Element<Item, Key>>& elementsIterator,
																		size_t elementsCount)
{
	assert(elements.getCount() == elementsCount);

	for (size_t index = 0; index < elementsCount; ++index)
	{
		assert(elementsIterator.isValid());
		const Element<Item, Key>& element = elementsIterator.getCurrentItem();

		assert(element.item != nullptr);
		setElementAtWith(index, element);

		elementsIterator.advance();
	}

	this->elementsCount = elementsCount;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::buildHeap()
{
	for (long nonLeaf = (elementsCount / 2) - 1; nonLeaf >= 0; --nonLeaf)
	{
		siftDownElementAt(nonLeaf);
	}
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::PriorityQueue(PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>&& source) :
	elements(std::move(source.elements)),
	elementsCount(source.elementsCount),
	handleUpdator(std::move(source.handleUpdator)),
	keyAccessor(std::move(source.keyAccessor))
{
	source.elementsCount = 0;
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
	std::swap(elements, priorityQueue.elements);
	std::swap(elementsCount, priorityQueue.elementsCount);
	std::swap(handleUpdator, priorityQueue.handleUpdator);
	std::swap(keyAccessor, priorityQueue.keyAccessor);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::decreaseKey(const PriorityQueueHandle& handleToElement,
																	   const Key& newKey)
{
	assert(handleToElement.isValid());
	
	size_t index = handleToElement.index;
	assert(isIndexWithinHeap(index));

	setKeyAtWith(index, newKey);
	siftUpElementAt(index);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::setKeyAtWith(size_t index, const Key& newKey)
{
	Element<Item, Key>& element = elements[index];

	assert(!(element.key < newKey));
	element.key = newKey;

	Item& item = *element.item;

	assert(!(keyAccessor.getKeyOf(item) < newKey));
	keyAccessor.setKeyOfWith(item, newKey);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::add(const Element<Item, Key>& element)
{
	assert(element.item != nullptr);
	
	addAtEnd(element);
	siftUpElementAt(elementsCount - 1);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::addAtEnd(const Element<Item, Key>& newElement)
{
	if (elementsCount < elements.getCount())
	{
		elements[elementsCount] = newElement;
	}
	else
	{
		elements.add(newElement);
	}

	++elementsCount;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::siftUpElementAt(size_t index)
{
	const Element<Item, Key> elementToMove(elements[index]);
	size_t parent;

	while (!isRoot(index))
	{
		parent = computeParentOf(index);

		if (compare(*(elementToMove.item), *(elements[parent].item)))
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

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline typename PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::Element<Item, Key>
PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::getMinElement() const
{
	assert(!isEmpty());

	return elements[0];
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
typename PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::Element<Item, Key>
PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::extractMinElement()
{
	assert(!isEmpty());

	invalidateHandleOfElementAt(0);

	Element<Item, Key> minElement = elements[0];
	--elementsCount;

	if (!isEmpty())
	{
		setElementAtWith(0, elements[elementsCount]);
		siftDownElementAt(0);
	}

	return minElement;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::siftDownElementAt(size_t index)
{
	const Element<Item, Key> elementToMove(elements[index]);
	size_t successor = computeLeftChildOf(index);

	while (isIndexWithinHeap(successor))
	{
		successor = computeIndexOfMinKeySuccessor(successor);

		if (compare(*(elements[successor].item), *(elementToMove.item)))
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

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
size_t PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::computeIndexOfMinKeySuccessor(size_t leftSuccessor) const
{
	assert(isIndexWithinHeap(leftSuccessor));
	size_t rightSuccessor = leftSuccessor + 1;

	if (isIndexWithinHeap(rightSuccessor)
		&& compare(*(elements[rightSuccessor].item), *(elements[leftSuccessor].item)))
	{
		return rightSuccessor;
	}
	else
	{
		return leftSuccessor;
	}
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::setElementAtWith(size_t index,
																				   const Element<Item, Key>& element)
{
	elements[index] = element;
	setKeyAtWith(index, element.key);
	setHandleOfElementAtWith(index, PriorityQueueHandle(index));
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::invalidateHandleOfElementAt(size_t index)
{
	setHandleOfElementAtWith(index, PriorityQueueHandle(-1));
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline void PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::setHandleOfElementAtWith(size_t index,
																						   const PriorityQueueHandle& handle)
{
	handleUpdator(*(elements[index].item), handle);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline bool PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::compare(const Item& lhs, const Item& rhs) const
{
	return keyAccessor.getKeyOf(lhs) < keyAccessor.getKeyOf(rhs);
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline bool PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::isEmpty() const
{
	return elementsCount == 0;
}

template <class Item, class Key, class HandleUpdator, class KeyAccessor>
inline bool PriorityQueue<Item, Key, HandleUpdator, KeyAccessor>::isIndexWithinHeap(size_t index) const
{
	return index < elementsCount;
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
