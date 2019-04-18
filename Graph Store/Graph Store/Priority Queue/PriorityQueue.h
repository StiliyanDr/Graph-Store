#ifndef __PRIORITY_QUEUE_HEADER_INCLUDED__
#define __PRIORITY_QUEUE_HEADER_INCLUDED__

#include "Dynamic Array/DynamicArray.h"
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

class IdentityKeyAccessor
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
	class Comparator = Less,
	class Key = Item,
	class KeyAccessor = IdentityKeyAccessor,
	class HandleUpdator = EmptyMethodFunctor>
class PriorityQueue
{
	typedef PriorityQueueHandle Handle;

	class Element
	{
	public:
		static bool compare(const Element& lhs, const Element& rhs);
		
	public:
		Element(const Item& item = Item());

		void invalidateHandle();
		void setHandle(const Handle& handle);
		void optimiseKey(const Key& newKey);
		const Key& getKey() const;
		const Item& getItem() const;

	private:
		static HandleUpdator handleUpdator;
		static KeyAccessor keyAccessor;
		static Comparator comparator;

	private:
		Item item;
	};

public:
	PriorityQueue() = default;

	template <class Iterator>
	PriorityQueue(Iterator& iterator, size_t itemsCount);
	PriorityQueue(const PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&) = default;
	PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&
		operator=(const PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&) = default;
	PriorityQueue(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&&) = default;
	PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&
		operator=(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator>&& rhs);
	~PriorityQueue();

	void add(const Item& item);
	Item extractOptimal();
	Item getOptimal() const;
	void optimiseKey(const Handle& handle, const Key& newKey);
	void empty();
	bool isEmpty() const;

private:
	static size_t computeLeftChildOf(size_t index);
	static size_t computeParentOf(size_t index);
	static bool isRoot(size_t index);

private:
	void buildHeap();
	void siftDownElementAt(size_t index);
	void siftUpElementAt(size_t index);
	void moveLastElementAtTopOfHeap();
	void addAtEnd(const Element& element);
	void setElementAtWith(size_t index, const Element& element);
	void invalidateAllHandles();
	size_t computeOptimalKeySuccessor(size_t leftSuccessor) const;
	bool isWithinHeap(size_t index) const;
	template <class Iterator>
	void copyItems(Iterator& iterator, size_t itemsCount);
	void verifyQueueIsNotEmpty() const;
	void verifyHandleValidity(const Handle& h) const;
	void swapContentsWith(PriorityQueue<Item, Comparator, Key, KeyAccessor, HandleUpdator> queue);

private:
	DynamicArray<Element> elements;
};

#include "PriorityQueue.hpp"

#endif //__PRIORITY_QUEUE_HEADER_INCLUDED__