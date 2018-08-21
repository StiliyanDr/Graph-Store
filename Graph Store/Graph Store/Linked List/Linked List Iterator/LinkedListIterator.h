#ifndef __LINKED_LIST_ITERATOR_HEADER_INCLUDED__
#define __LINKED_LIST_ITERATOR_HEADER_INCLUDED__

#include "../Box.h"
#include "../../Iterator/Iterator.h"

template <class T>
class LinkedList;

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
	friend bool operator==(const LinkedListIterator<Item>& lhs, const LinkedListIterator<Item>& rhs);
	
private:
	LinkedListIterator(Box<Item>* current, const LinkedList<Item>* owner);

	Item& getCurrentItem();
	bool isValid() const;

private:
	Box<Item>* current;
	const LinkedList<Item>* owner;
};

template <class Item>
bool operator!=(const LinkedListIterator<Item>& lhs, const LinkedListIterator<Item>& rhs);

#include "LinkedListIterator.hpp"

#endif //__LINKED_LIST_ITERATOR_HEADER_INCLUDED__