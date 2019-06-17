#ifndef __PRIORITY_QUEUE_HEADER_INCLUDED__
#define __PRIORITY_QUEUE_HEADER_INCLUDED__

#include <vector>
#include <type_traits>
#include "PriorityQueueHandle.h"

class Less
{
public:
	template <class T>
	bool operator()(const T& lhs, const T& rhs) const
	noexcept(noexcept(lhs < rhs))
	{
		return lhs < rhs;
	}
};

class NoOpHandleUpdator
{
public:
	template <class Item>
	void operator()(Item&, const PriorityQueueHandle&)
	const noexcept
	{
	}
};

class IdentityKeyAccessor
{
public:
	template <class Item>
	const Item& getKeyOf(const Item& item)
	const noexcept
	{
		return item;
	}

	template <class Item, class Key>
	void setKeyOfWith(Item& item, Key&& key) const
	{
		item = std::forward<Key>(key);
	}
};

template <class Item,
	class Comparator = Less,
	class Key = Item,
	class KeyAccessor = IdentityKeyAccessor,
	class HandleUpdator = NoOpHandleUpdator>
class PriorityQueue
{
	using SizeType = std::size_t;
	using Handle = PriorityQueueHandle;

	static_assert((std::is_nothrow_copy_constructible_v<Item>
		           && std::is_nothrow_copy_assignable_v<Item>)
		          ||
		          (std::is_nothrow_move_constructible_v<Item>
			       && std::is_nothrow_move_assignable_v<Item>),
		          "Item must have safe copy or move operations!");
	static_assert(std::is_nothrow_invocable_r_v<bool,
		                                        Comparator,
		                                        const Item&,
		                                        const Item&>,
		          "Comparator must be nothrow-invocable!");
	static_assert(std::is_nothrow_invocable_r_v<const Key&,
		                                        decltype(&KeyAccessor::getKeyOf),
		                                        const KeyAccessor*,
		                                        const Item&>,
		          "KeyAccessor::getKeyOf must be nothrow-invocable!");
	static_assert(std::is_nothrow_invocable_v<HandleUpdator,
		                                      Item&,
		                                      const Handle&>,
		          "HandleUpdator must be nothrow-invocable!");

	class Element
	{
	public:
		static bool compare(const Element& lhs,
			                const Element& rhs) noexcept
		{
			return comparator(lhs.getKey(), rhs.getKey());
		}

	public:
		explicit Element(Item&& item)
		noexcept(std::is_nothrow_move_constructible_v<Item>) :
			item(std::move(item))
		{
		}

		explicit Element(const Item& item)
		noexcept(std::is_nothrow_copy_constructible_v<Item>) :
			item(item)
		{
		}

		void invalidateHandle() noexcept
		{
			setHandle(Handle{});
		}

		void setHandle(const Handle& h) noexcept
		{
			handleUpdator(item, h);
		}

		template <class KeyType>
		void optimiseKey(KeyType&& newKey)
		{
			if (!comparator(getKey(), newKey))
			{
				keyAccessor.setKeyOfWith(item,
					                     std::forward<KeyType>(newKey));
			}
			else
			{
				throw std::invalid_argument("The key can't be worsened!");
			}
		}

		const Key& getKey() const noexcept
		{
			return keyAccessor.getKeyOf(item);
		}

		const Item& getItem() const noexcept
		{
			return item;
		}

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
	PriorityQueue(Iterator begin, Iterator end);
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
	static SizeType computeLeftChildOf(SizeType index) noexcept;
	static SizeType computeParentOf(SizeType index) noexcept;
	static bool isRoot(SizeType index) noexcept;

private:
	template <class ItemType>
	void doAdd(ItemType&& item);
	template <class KeyType>
	void doOptimiseKey(const Handle& h, KeyType&& newKey);
	void buildHeap() noexcept;
	void siftDownElementAt(SizeType index) noexcept;
	void siftUpElementAt(SizeType index) noexcept;
	void moveLastElementAtTopOfHeap();
	template <class ItemType>
	void addAtEnd(ItemType&& item);
	void setElementAtWith(SizeType index, Element& element) noexcept;
	void updateHandleAt(SizeType index) noexcept;
	void invalidateAllHandles();
	SizeType computeOptimalKeyChild(SizeType leftChild) const noexcept;
	bool isWithinHeap(SizeType index) const noexcept;
	template <class Iterator>
	void copyItems(Iterator begin, Iterator end);
	void verifyQueueIsNotEmpty() const;
	void verifyHandleValidity(const Handle& h) const;
	void swapContentsWith(PriorityQueue q);

private:
	std::vector<Element> elements;
};

#include "PriorityQueue.hpp"

#endif //__PRIORITY_QUEUE_HEADER_INCLUDED__