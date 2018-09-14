#ifndef __PRIORITY_QUEUE_HEADER_INCLUDED__
#define __PRIORITY_QUEUE_HEADER_INCLUDED__

#include "../Dynamic Array/DynamicArray.h"
#include "../Iterator/Iterator.h"
#include "Handle/PriorityQueueHandle.h"

class Less
{
public:
	template <class T>
	bool operator()(const T& lhs, const T& rhs) const
	{
		return lhs < rhs;
	}
};

class EmptyMethodFunctor
{
public:
	template <class Item>
	void operator()(Item&, const PriorityQueueHandle&) const
	{
	}
};

class Identity
{
public:
	template <class Item>
	const Item& getKeyOf(const Item& object) const
	{
		return object;
	}

	template <class Item>
	void setKeyOfWith(Item& item, const Item& key) const
	{
		item = key;
	}
};

template <class Item,
	class Comparator = std::less,
	class Key = Item,
	class KeyAccessor = Identity,
	class HandleUpdator = EmptyMethodFunctor>
class PriorityQueue
{
public:
	PriorityQueue();
	PriorityQueue(Iterator<Item*>& itemsIterator, size_t itemsCount);
	PriorityQueue(const PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&) = default;
	PriorityQueue(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&& source);
	PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&
		operator=(const PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&) = default;
	PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&
		operator=(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&& rhs);
	~PriorityQueue();

	void add(Item* item);
	Item* extractMin();
	const Item* getMin() const;
	void decreaseKey(const PriorityQueueHandle& handleToItem, const Key& newKey);
	void empty();
	bool isEmpty() const;

private:
	static size_t computeLeftChildOf(size_t index);
	static size_t computeParentOf(size_t index);
	static bool isRoot(size_t index);

private:
	void buildHeap();
	void siftDownItemAt(size_t index);
	void siftUpItemAt(size_t index);
	void addAtEnd(Item* newItem);
	void setItemAtWith(size_t index, Item* item);
	void invalidateAllHandles();
	void invalidateHandleAt(size_t index);
	void setHandleAtWith(size_t index, const PriorityQueueHandle& handle);
	void setKeyAtWith(size_t index, const Key& newKey);
	size_t computeMinKeySuccessor(size_t leftSuccessor) const;
	bool compare(const Item& lhs, const Item& rhs) const;
	bool isWithinHeap(size_t index) const;
	void copyItems(Iterator<Item*>& itemsIterator, size_t itemsCount);
	void swapContentsWith(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator> queue);

private:
	DynamicArray<Item*> items;
	size_t itemsCount;
	HandleUpdator handleUpdator;
	KeyAccessor keyAccessor;
	Comparator comparator;
};

#include "PriorityQueue.hpp"

#endif //__PRIORITY_QUEUE_HEADER_INCLUDED__