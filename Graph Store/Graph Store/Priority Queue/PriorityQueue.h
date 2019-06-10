#ifndef __PRIORITY_QUEUE_HEADER_INCLUDED__
#define __PRIORITY_QUEUE_HEADER_INCLUDED__

#include "Dynamic Array/DynamicArray.h"

class PriorityQueueHandle;

class Less
{
public:
	template <class T>
	bool operator()(const T& lhs, const T& rhs) const
	{
		return lhs < rhs;
	}
};

class EmptyFunction
{
public:
	template <class Item>
	void operator()(Item&, const PriorityQueueHandle&) const noexcept
	{
	}
};

class IdentityKeyAccessor
{
public:
	template <class Item>
	const Item& getKeyOf(const Item& item) const noexcept
	{
		return item;
	}

	template <class Item>
	void setKeyOfWith(Item& item, Item&& key) const
	{
		item = std::forward<Item>(key);
	}
};

template <class Item,
	class Comparator = Less,
	class Key = Item,
	class KeyAccessor = IdentityKeyAccessor,
	class HandleUpdator = EmptyFunction>
class PriorityQueue
{
public:
	using SizeType = std::size_t;

private:
	using Handle = PriorityQueueHandle;

	class Element
	{
	public:
		static bool compare(const Element& lhs, const Element& rhs);
		
	public:
		Element() = default;
		template <class ItemType>
		explicit Element(ItemType&& item);

		void invalidateHandle();
		void setHandle(const Handle& h);
		template <class KeyType>
		void optimiseKey(KeyType&& newKey);
		const Key& getKey() const;
		const Item& getItem() const noexcept;

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
	PriorityQueue(Iterator iterator, SizeType itemsCount);
	PriorityQueue(const PriorityQueue&) = default;
	PriorityQueue& operator=(const PriorityQueue&) = default;
	PriorityQueue(PriorityQueue&&) = default;
	PriorityQueue& operator=(PriorityQueue&& rhs);
	~PriorityQueue();

	void add(const Item& item);
	void add(Item&& item);
	Item extractOptimal();
	Item getOptimal() const;
	void optimiseKey(const Handle& h, const Key& newKey);
	void optimiseKey(const Handle& h, Key&& newKey);
	void empty();
	bool isEmpty() const noexcept;

private:
	static SizeType computeLeftChildOf(SizeType index);
	static SizeType computeParentOf(SizeType index);
	static bool isRoot(SizeType index);

private:
	template <class ItemType>
	void doAdd(ItemType&& item);
	template <class KeyType>
	void doOptimiseKey(const Handle& h, KeyType&& newKey);
	void buildHeap();
	void siftDownElementAt(SizeType index);
	void siftUpElementAt(SizeType index);
	void moveLastElementAtTopOfHeap();
	void addAtEnd(Element&& element);
	void setElementAtWith(SizeType index, Element& element);
	void invalidateAllHandles();
	SizeType computeOptimalKeyChild(SizeType leftChild) const;
	bool isWithinHeap(SizeType index) const;
	template <class Iterator>
	void copyItems(Iterator iterator, SizeType itemsCount);
	void verifyQueueIsNotEmpty() const;
	void verifyHandleValidity(const Handle& h) const;
	void swapContentsWith(PriorityQueue q);

private:
	DynamicArray<Element> elements;
};

#include "PriorityQueue.hpp"

#endif //__PRIORITY_QUEUE_HEADER_INCLUDED__