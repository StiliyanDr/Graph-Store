#ifndef __LINKED_LIST_HEADER_INCLUDED__
#define __LINKED_LIST_HEADER_INCLUDED__

#include "LinkedListIterator.h"

template <class T>
class LinkedList
{
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

	LinkedListIterator<T> search(const T& what);
	LinkedListIterator<T> getIteratorToFirst();
	LinkedListIterator<T> getIteratorToLast();

	void empty();
	bool isEmpty() const;
	size_t getSize() const;

	const T& getFirst() const;
	const T& getLast() const;

private:
	static Box<T>* findEndOfChain(Box<T>* current);
	static void destroyChain(Box<T>* first);

private:
	void appendChainFrom(LinkedList<T> source);
	void insertAfter(Box<T>* iterator, const T& item);
	void insertBefore(Box<T>* iterator, const T& item);
	void removeAt(Box<T>* iterator);
	Box<T>* findBoxBefore(const Box<T>* box) const;
	void copyFrom(const LinkedList<T>& source);
	void copyChainFrom(const LinkedList<T>& source);
	void swapContentsWith(LinkedList<T> list);
	void nullifyMembers();
	bool isOwnerOf(const LinkedListIterator<T>& iterator) const;

private:
	size_t size;
	Box<T>* first;
	Box<T>* last;
};

#include "LinkedList.hpp"

#endif //__LINKED_LIST_HEADER_INCLUDED__