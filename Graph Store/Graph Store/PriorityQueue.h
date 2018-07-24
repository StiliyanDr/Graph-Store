#ifndef __PRIORITY_QUEUE_HEADER_INCLUDED__
#define __PRIORITY_QUEUE_HEADER_INCLUDED__

#include "DynamicArray.h"
#include "Iterator.h"

template <class Item, class Key, class HandleUpdator>
class PriorityQueue
{
public:
	template <class Item, class Key>
	struct Element
	{
		Element(Item* item, const Key& key) :
			item(item), key(key) { }

		Item* item;
		Key key;
	};

	class Handle
	{
	public:
		Handle();
	
	private:
		Handle(long index);
		bool isValid() const;

	private:
		long index;
	};

public:
	PriorityQueue();
	PriorityQueue(Iterator<Element<Item, Key>>& elementsIterator, size_t elementsCount);
	PriorityQueue(const PriorityQueue<Item, Key, HandleUpdator>&) = default;
	PriorityQueue(PriorityQueue<Item, Key, HandleUpdator>&& source);
	PriorityQueue<Item, Key, HandleUpdator>& operator=(const PriorityQueue<Item, Key, HandleUpdator>&) = default;
	PriorityQueue<Item, Key, HandleUpdator>& operator=(PriorityQueue<Item, Key, HandleUpdator>&& rhs);
	~PriorityQueue() = default;

	void insert(const Element<Item, Key>& element);
	Element<Item, Key> extractMinElement();
	Element<Item, Key> getMinElement() const;
	void decreaseKey(const Handle& handleToElement, const Key& newKey);
	bool isEmpty() const;

private:
	static size_t computeLeftChildOf(size_t index);
	static size_t computeParentOf(size_t index);
	static bool isRoot(size_t index);

private:
	void buildHeap();
	void siftDownElementAt(size_t index);
	void siftUpElementAt(size_t index);
	void insertAtEnd(const Element<Item, Key>& newElement);
	void setElementAtWith(size_t index, const Element<Item, Key>& element);
	void invalidateHandleOfElementAt(size_t index);
	void setHandleOfElementAtWith(size_t index, const Handle& handle);
	void setKeyAtWith(size_t index, const Key& newKey);
	size_t computeIndexOfMinKeySuccessor(size_t leftSuccessor) const;
	bool isIndexWithinHeap(size_t index) const;
	void copyElements(Iterator<Element<Item, Key>>& elementsIterator, size_t elementsCount);
	void swapContentsWith(PriorityQueue<Item, Key, HandleUpdator> priorityQueue);

private:
	DynamicArray<Element<Item, Key>> elements;
	size_t elementsCount;
	HandleUpdator handleUpdator;
};

#include "Handle.hpp"
#include "PriorityQueue.hpp"

#endif //__PRIORITY_QUEUE_HEADER_INCLUDED__