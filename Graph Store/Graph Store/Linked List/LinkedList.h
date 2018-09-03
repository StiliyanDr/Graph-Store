#ifndef __LINKED_LIST_HEADER_INCLUDED__
#define __LINKED_LIST_HEADER_INCLUDED__

#include <type_traits>

template <class T>
class LinkedList
{
	template <class T>
	struct Box
	{
		Box(const T& item, Box<T>* next = nullptr) :
			item(item), next(next) { }

		Box<T>* next;
		T item;
	};

public:
	template <class Item, bool isConst = false>
	class LinkedListIterator
	{
		friend class LinkedList<Item>;
	
	public:
		typedef typename std::conditional<isConst, const Item&, Item&>::type Reference;
		typedef typename std::conditional<isConst, const Item*, Item*>::type Pointer;

	public:
		LinkedListIterator(const LinkedListIterator<Item, false>&) = default;

		LinkedListIterator<Item, isConst>& operator++();
		LinkedListIterator<Item, isConst> operator++(int);
		Reference operator*() const;
		Pointer operator->() const;
		bool operator!() const;
		operator bool() const;

		template <class Item, bool isConst>
		friend bool operator==(typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
							   typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs);

	private:
		LinkedListIterator(Box<Item>* current, const LinkedList<Item>* owner);

		Reference getCurrentItem() const;
		bool isValid() const;

	private:
		Box<Item>* current;
		const LinkedList<Item>* owner;
	};

	typedef LinkedListIterator<T, false> Iterator;
	typedef LinkedListIterator<T, true> ConstIterator;

public:
	LinkedList();
	LinkedList(LinkedList<T>&& source);
	LinkedList(const LinkedList<T>& source);
	LinkedList<T>& operator=(LinkedList<T>&& rhs);
	LinkedList<T>& operator=(const LinkedList<T>& rhs);
	~LinkedList();

	void appendList(const LinkedList<T>& list);
	void appendList(LinkedList<T>&& list);

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
	size_t getSize() const;

	const T& getFirst() const;
	const T& getLast() const;

private:
	static void destroyChain(Box<T>* firstBox);

private:
	void appendChainFrom(LinkedList<T> source);
	void insertAfter(Box<T>* box, const T& item);
	void insertBefore(Box<T>* box, const T& item);
	void removeAt(Box<T>* box);
	Box<T>* findBoxBefore(const Box<T>* box) const;
	void copyFrom(const LinkedList<T>& source);
	void copyChainFrom(const LinkedList<T>& source);
	void swapContentsWith(LinkedList<T> list);
	void nullifyMembers();
	void verifyOwnershipOf(const Iterator& iterator) const;
	void verifyThatListIsNotEmpty() const;

private:
	size_t size;
	Box<T>* first;
	Box<T>* last;
};

template <class Item, bool isConst>
bool operator!=(typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& lhs,
				typename const LinkedList<Item>::LinkedListIterator<Item, isConst>& rhs);

#include "LinkedListIterator.hpp"
#include "LinkedList.hpp"

#endif //__LINKED_LIST_HEADER_INCLUDED__