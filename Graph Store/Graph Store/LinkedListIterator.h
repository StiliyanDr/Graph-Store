#ifndef __LINKED_LIST_ITERATOR_HEADER_INCLUDED__
#define __LINKED_LIST_ITERATOR_HEADER_INCLUDED__

#include "Box.h"
#include "Iterator.h"

template <class T>
class LinkedList;

template <class T>
class LinkedListIterator : public Iterator<T>
{
	friend class LinkedList<T>;

public:
	virtual ~LinkedListIterator() = default;

	virtual void advance() override;
	virtual T& getCurrentItem() override;
	virtual bool isValid() const override;

	LinkedListIterator<T>& operator++();
	LinkedListIterator<T> operator++(int);

	T& operator*();
	bool operator!() const;
	operator bool() const;

	template <class T>
	friend bool operator==(const LinkedListIterator<T>& lhs, const LinkedListIterator<T>& rhs);
	
	template <class T>
	friend bool operator!=(const LinkedListIterator<T>& lhs, const LinkedListIterator<T>& rhs);

private:
	LinkedListIterator(Box<T>* current, const LinkedList<T>* owner);

private:
	Box<T>* current;
	const LinkedList<T>* owner;
};

#include "LinkedListIterator.hpp"

#endif //__LINKED_LIST_ITERATOR_HEADER_INCLUDED__