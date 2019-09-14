#ifndef __LINKED_LIST_HEADER_INCLUDED__
#define __LINKED_LIST_HEADER_INCLUDED__

#include "Iterator/Iterator.h"

template <class T>
class LinkedList
{
	template <class T>
	struct Node
	{
        template <class U>
		Node(U&& item, Node<T>* next = nullptr) :
            next(next),
			item(std::forward<U>(item))
        {
        }

		Node<T>* next;
		T item;
	};

public:
	template <class Item, bool isConst = false>
	class LinkedListIterator : public AbstractIterator<Item, isConst>
	{
		friend class LinkedList<Item>;
	
	public:
		using typename AbstractIterator<Item, isConst>::Reference;

	public:
		LinkedListIterator(const LinkedListIterator<Item, false>& source) noexcept;

		LinkedListIterator& operator++() noexcept override;
		const LinkedListIterator operator++(int) noexcept;

		template <class Item, bool isConst>
		friend bool operator==(typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
							   typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs) noexcept;

	private:
		LinkedListIterator(Node<Item>* current,
			               const LinkedList<Item>* owner) noexcept;

		Reference getCurrentItem() const noexcept override;
		bool isValid() const noexcept override;

	private:
		Node<Item>* current;
		const LinkedList<Item>* owner;
	};

	using Iterator = LinkedListIterator<T, false>;
	using ConstIterator = LinkedListIterator<T, true>;
    using SizeType = std::size_t;

public:
	LinkedList() noexcept;
	LinkedList(LinkedList&& source) noexcept;
	LinkedList(const LinkedList& source);
	LinkedList& operator=(LinkedList&& rhs) noexcept;
	LinkedList& operator=(const LinkedList& rhs);
	~LinkedList();

	void appendList(LinkedList list) noexcept;

	void insertAfter(const Iterator& iterator, const T& item);
    void insertAfter(const Iterator& iterator, T&& item);
	void insertBefore(const Iterator& iterator, const T& item);
    void insertBefore(const Iterator& iterator, T&& item);
	void addFront(const T& item);
    void addFront(T&& item);
	void addBack(const T& item);
    void addBack(T&& item);

	void removeAt(Iterator& iterator);
	void removeAfter(const Iterator& iterator);
	void removeBefore(const Iterator& iterator);
	void removeFirst();
	void removeLast();

	ConstIterator getConstIterator() const noexcept;
	Iterator getIteratorToFirst() noexcept;
	Iterator getIteratorToLast() noexcept;

	void empty() noexcept;
	bool isEmpty() const noexcept;
	SizeType getSize() const noexcept;

	const T& getFirst() const;
	const T& getLast() const;

private:
    template <class U>
    void doInsertAfter(const Iterator& iterator, U&& item);
    template <class U>
    void doInsertBefore(const Iterator& iterator, U&& item);
    template <class U>
    void doAddFront(U&& item);
    template <class U>
    void doAddBack(U&& item);
    template <class U>
    void insertAfter(Node<T>* node, U&& item);
	template <class U>
    void insertBefore(Node<T>* node, U&& item);
	void removeAt(Node<T>* node) noexcept;
	Node<T>* findNodeBefore(const Node<T>* node) noexcept;
	void copyChainFrom(const LinkedList& source);
	void swapContentsWith(LinkedList list) noexcept;
	void nullifyMembers() noexcept;
	void verifyOwnershipOf(const Iterator& iterator) const;
	void verifyThatListIsNotEmpty() const;
	void destroyChain() noexcept;

private:
	SizeType size;
	Node<T>* first;
	Node<T>* last;
};

template <class Item, bool isConst>
bool operator!=(typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
				typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs) noexcept;

#include "LinkedListIterator.hpp"
#include "LinkedList.hpp"

#endif //__LINKED_LIST_HEADER_INCLUDED__