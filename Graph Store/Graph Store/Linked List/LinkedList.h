#ifndef __LINKED_LIST_HEADER_INCLUDED__
#define __LINKED_LIST_HEADER_INCLUDED__

#include <type_traits>

template <class T>
class LinkedList
{
	template <class T>
	struct Node
	{
		Node(const T& item, Node<T>* next = nullptr) :
			item(item), next(next) { }

		Node<T>* next;
		T item;
	};

public:
	template <class Item, bool isConst = false>
	class LinkedListIterator
	{
		friend class LinkedList<Item>;
	
	public:
		using Reference = std::conditional_t<isConst, const Item&, Item&>;
		using Pointer = std::conditional_t<isConst, const Item*, Item*>;

	public:
		LinkedListIterator(const LinkedListIterator<Item, false>& source) noexcept;

		LinkedListIterator<Item, isConst>& operator++() noexcept;
		LinkedListIterator<Item, isConst> operator++(int) noexcept;
		Reference operator*() const;
		Pointer operator->() const;
		bool operator!() const noexcept;
		operator bool() const noexcept;

		template <class Item, bool isConst>
		friend bool operator==(typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
							   typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs) noexcept;

	private:
		LinkedListIterator(Node<Item>* current,
			               const LinkedList<Item>* owner) noexcept;

		Reference getCurrentItem() const;
		bool isValid() const noexcept;

	private:
		Node<Item>* current;
		const LinkedList<Item>* owner;
	};

	using Iterator = LinkedListIterator<T, false>;
	using ConstIterator = LinkedListIterator<T, true>;

public:
	LinkedList();
	LinkedList(LinkedList<T>&& source);
	LinkedList(const LinkedList<T>& source);
	LinkedList<T>& operator=(LinkedList<T>&& rhs);
	LinkedList<T>& operator=(const LinkedList<T>& rhs);
	~LinkedList();

	void appendList(LinkedList<T> list) noexcept;

	void insertAfter(const Iterator& iterator, const T& item);
	void insertBefore(const Iterator& iterator, const T& item);
	void addFront(const T& item);
	void addBack(const T& item);

	void removeAt(Iterator& iterator);
	void removeAfter(const Iterator& iterator);
	void removeBefore(const Iterator& iterator);
	void removeFirst();
	void removeLast();

	ConstIterator getConstIterator() const;
	Iterator getIteratorToFirst();
	Iterator getIteratorToLast();

	void empty();
	bool isEmpty() const;
	std::size_t getSize() const;

	const T& getFirst() const;
	const T& getLast() const;

private:
	void insertAfter(Node<T>* node, const T& item);
	void insertBefore(Node<T>* node, const T& item);
	void removeAt(Node<T>* node);
	Node<T>* findNodeBefore(const Node<T>* node);
	void copyFrom(const LinkedList<T>& source);
	void copyChainFrom(const LinkedList<T>& source);
	void swapContentsWith(LinkedList<T> list);
	void nullifyMembers();
	void verifyOwnershipOf(const Iterator& iterator) const;
	void verifyThatListIsNotEmpty() const;
	void destroyChain();

private:
	std::size_t size;
	Node<T>* first;
	Node<T>* last;
};

template <class Item, bool isConst>
bool operator!=(typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
				typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs) noexcept;

#include "LinkedListIterator.hpp"
#include "LinkedList.hpp"

#endif //__LINKED_LIST_HEADER_INCLUDED__