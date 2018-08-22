#ifndef __LINKED_LIST_HEADER_INCLUDED__
#define __LINKED_LIST_HEADER_INCLUDED__

#include "../Iterator/Iterator.h"

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
	template <class Item>
	class LinkedListIterator : public Iterator<Item>
	{
		friend class LinkedList<Item>;

	public:
		virtual LinkedListIterator<Item>& operator++() override;
		virtual Item& operator*() override;
		virtual Item* operator->() override;
		virtual bool operator!() const override;
		virtual operator bool() const override;

		template <class Item>
		friend bool operator==(typename const LinkedList<Item>::LinkedListIterator<Item>& lhs,
							   typename const LinkedList<Item>::LinkedListIterator<Item>& rhs);

	private:
		LinkedListIterator(Box<Item>* current, const LinkedList<Item>* owner);

		Item& getCurrentItem();
		bool isValid() const;

	private:
		Box<Item>* current;
		const LinkedList<Item>* owner;
	};

public:
	LinkedList();
	LinkedList(LinkedList<T>&& source);
	LinkedList(const LinkedList<T>& source);
	LinkedList<T>& operator=(LinkedList<T>&& rhs);
	LinkedList<T>& operator=(const LinkedList<T>& rhs);
	~LinkedList();

	void appendList(const LinkedList<T>& list);
	void appendList(LinkedList<T>&& list);

	void insertAfter(LinkedListIterator<T>& iterator, const T& item);
	void insertBefore(LinkedListIterator<T>& iterator, const T& item);
	void addFront(const T& item);
	void addBack(const T& item);

	void removeAt(LinkedListIterator<T>& iterator);
	void removeAfter(LinkedListIterator<T>& iterator);
	void removeBefore(LinkedListIterator<T>& iterator);
	void removeFirst();
	void removeLast();

	LinkedListIterator<T> getIteratorToFirst();
	LinkedListIterator<T> getIteratorToLast();

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
	void verifyOwnershipOf(const LinkedListIterator<T>& iterator) const;
	void verifyThatListIsNotEmpty() const;

private:
	size_t size;
	Box<T>* first;
	Box<T>* last;
};

template <class Item>
bool operator!=(typename const LinkedList<Item>::LinkedListIterator<Item>& lhs,
				typename const LinkedList<Item>::LinkedListIterator<Item>& rhs);

#include "LinkedListIterator.hpp"
#include "LinkedList.hpp"

#endif //__LINKED_LIST_HEADER_INCLUDED__