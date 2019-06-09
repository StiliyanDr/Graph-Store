#include "PriorityQueueHandle.h"

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
HandleUpdator PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::handleUpdator;

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
KeyAccessor PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::keyAccessor;

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
Comparator PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::comparator;

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
template <class ItemType>
inline PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::Element(ItemType&& item) :
	item(std::forward<ItemType>(item))
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
template <class KeyType>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::optimiseKey(KeyType&& newKey)
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
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::setHandle(const Handle& h)
{
	handleUpdator(item, h);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline const Item& PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::Element::getItem() const noexcept
{
	return item;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
template <class Iterator>
PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::PriorityQueue(Iterator& iterator,
																				SizeType itemsCount) :
	PriorityQueue()
{
	copyItems(iterator, itemsCount);
	buildHeap();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
template <class Iterator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::copyItems(Iterator& iterator,
																				 SizeType itemsCount)
{
	elements.ensureSize(itemsCount);

	for (SizeType i = 1; i <= itemsCount; ++i)
	{
		addAtEnd(Element(*iterator));
		++iterator;
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::addAtEnd(const Element& element)
{
	elements.add(element);

	SizeType lastIndex = elements.getCount() - 1;
	elements[lastIndex].setHandle(Handle(lastIndex));
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
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::siftDownElementAt(SizeType index)
{
	Element elementToMove = elements[index];
	SizeType successor = computeLeftChildOf(index);

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
auto PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeOptimalKeySuccessor(SizeType leftSuccessor) const
-> SizeType
{
	assert(isWithinHeap(leftSuccessor));
	SizeType rightSuccessor = leftSuccessor + 1;

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
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::setElementAtWith(SizeType index,
																							   const Element& element)
{
	assert(isWithinHeap(index));

	elements[index] = element;
	elements[index].setHandle(Handle(index));
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
auto PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::operator=(PriorityQueue&& rhs)
-> PriorityQueue&
{
	if (this != &rhs)
	{
		invalidateAllHandles();
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::invalidateAllHandles()
{
	SizeType count = elements.getCount();

	for (SizeType i = 0; i < count; ++i)
	{
		elements[i].invalidateHandle();
	}
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::swapContentsWith(PriorityQueue q)
{
	std::swap(elements, q.elements);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::~PriorityQueue()
{
	invalidateAllHandles();
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
void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::siftUpElementAt(SizeType index)
{
	Element elementToMove = elements[index];
	SizeType parent;

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
inline void PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::add(const Item& item)
{
	addAtEnd(Element(item));
	siftUpElementAt(elements.getCount() - 1);
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

	SizeType indexOfLastElement = elements.getCount() - 1;
	setElementAtWith(0, elements[indexOfLastElement]);
	elements.removeAt(indexOfLastElement);
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isEmpty() const
{
	return elements.isEmpty();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isWithinHeap(SizeType index) const
{
	return index < elements.getCount();
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline auto PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeLeftChildOf(SizeType index)
-> SizeType
{
	return (2 * index) + 1;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline auto PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::computeParentOf(SizeType index)
-> SizeType
{
	assert(index > 0);

	return (index - 1) / 2;
}

template <class Item, class Comparator, class Key, class KeyAccessor, class HandleUpdator>
inline bool PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>::isRoot(SizeType index)
{
	return index == 0;
}
