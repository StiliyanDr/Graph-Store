
template <class Item, class Key, class HandleUpdator>
PriorityQueue<Item, Key, HandleUpdator>::PriorityQueue() :
	elementsCount(0)
{
}

template <class Item, class Key, class HandleUpdator>
PriorityQueue<Item, Key, HandleUpdator>::PriorityQueue(Iterator<Element<Item, Key>>& elementsIterator,
													   size_t elementsCount) :
	elements(elementsCount, elementsCount)
{
	copyElements(elementsIterator, elementsCount);
	buildHeap();
}

template <class Item, class Key, class HandleUpdator>
void PriorityQueue<Item, Key, HandleUpdator>::copyElements(Iterator<Element<Item, Key>>& elementsIterator,
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

template <class Item, class Key, class HandleUpdator>
void PriorityQueue<Item, Key, HandleUpdator>::decreaseKey(const Handle& handleToElement, const Key& newKey)
{
	assert(handleToElement.isValid());
	
	size_t index = handleToElement.index;
	assert(isIndexWithinHeap(index));

	setKeyAtWith(index, newKey);
	siftUpElementAt(index);
}

template <class Item, class Key, class HandleUpdator>
void PriorityQueue<Item, Key, HandleUpdator>::insert(const Element<Item, Key>& element)
{
	assert(element.item != nullptr);
	
	insertAtEnd(element);
	siftUpElementAt(elementsCount - 1);
}

template <class Item, class Key, class HandleUpdator>
typename PriorityQueue<Item, Key, HandleUpdator>::Element<Item, Key>
PriorityQueue<Item, Key, HandleUpdator>::extractMinElement()
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

template <class Item, class Key, class HandleUpdator>
void PriorityQueue<Item, Key, HandleUpdator>::buildHeap()
{
	for(long nonLeaf = (elementsCount / 2) - 1; nonLeaf >= 0; --nonLeaf)
	{
		siftDownElementAt(nonLeaf);
	}
}

template <class Item, class Key, class HandleUpdator>
void PriorityQueue<Item, Key, HandleUpdator>::siftDownElementAt(size_t index)
{
	const Element<Item, Key> elementToMove(elements[index]);
	size_t successor = computeLeftChildOf(index);

	while (isIndexWithinHeap(successor))
	{
		successor = computeIndexOfMinKeySuccessor(successor);

		if (elements[successor].key < elementToMove.key)
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

template <class Item, class Key, class HandleUpdator>
void PriorityQueue<Item, Key, HandleUpdator>::siftUpElementAt(size_t index)
{
	const Element<Item, Key> elementToMove(elements[index]);
	size_t parent;

	while (!isRoot(index))
	{
		parent = computeParentOf(index);

		if (elementToMove.key < elements[parent].key)
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

template <class Item, class Key, class HandleUpdator>
void PriorityQueue<Item, Key, HandleUpdator>::insertAtEnd(const Element<Item, Key>& newElement)
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

template <class Item, class Key, class HandleUpdator>
inline void PriorityQueue<Item, Key, HandleUpdator>::setElementAtWith(size_t index,
																	 const Element<Item, Key>& element)
{
	elements[index] = element;
	setHandleOfElementAtWith(index, Handle(index));
}

template <class Item, class Key, class HandleUpdator>
inline void PriorityQueue<Item, Key, HandleUpdator>::invalidateHandleOfElementAt(size_t index)
{
	setHandleOfElementAtWith(index, Handle(-1));
}

template <class Item, class Key, class HandleUpdator>
inline void PriorityQueue<Item, Key, HandleUpdator>::setHandleOfElementAtWith(size_t index, const Handle& handle)
{
	handleUpdator(*(elements[index].item), handle);
}

template <class Item, class Key, class HandleUpdator>
void PriorityQueue<Item, Key, HandleUpdator>::setKeyAtWith(size_t index, const Key& newKey)
{
	Element<Item, Key>& element = elements[index];

	assert(!(element.key < newKey));
	element.key = newKey;
}

template <class Item, class Key, class HandleUpdator>
size_t PriorityQueue<Item, Key, HandleUpdator>::computeIndexOfMinKeySuccessor(size_t leftSuccessor) const
{
	assert(isIndexWithinHeap(leftSuccessor));
	size_t rightSuccessor = leftSuccessor + 1;

	if (isIndexWithinHeap(rightSuccessor) 
		&& elements[rightSuccessor].key < elements[leftSuccessor].key)
	{
		return rightSuccessor;
	}
	else
	{
		return leftSuccessor;
	}
}

template <class Item, class Key, class HandleUpdator>
inline typename PriorityQueue<Item, Key, HandleUpdator>::Element<Item, Key>
PriorityQueue<Item, Key, HandleUpdator>::getMinElement() const
{
	assert(!isEmpty());

	return elements[0];
}

template <class Item, class Key, class HandleUpdator>
inline bool PriorityQueue<Item, Key, HandleUpdator>::isEmpty() const
{
	return elementsCount == 0;
}

template <class Item, class Key, class HandleUpdator>
inline bool PriorityQueue<Item, Key, HandleUpdator>::isIndexWithinHeap(size_t index) const
{
	return index < elementsCount;
}

template <class Item, class Key, class HandleUpdator>
inline size_t PriorityQueue<Item, Key, HandleUpdator>::computeLeftChildOf(size_t index)
{
	return (2 * index) + 1;
}

template <class Item, class Key, class HandleUpdator>
inline size_t PriorityQueue<Item, Key, HandleUpdator>::computeParentOf(size_t index)
{
	return (index - 1) / 2;
}

template <class Item, class Key, class HandleUpdator>
inline bool PriorityQueue<Item, Key, HandleUpdator>::isRoot(size_t index)
{
	return index == 0;
}
