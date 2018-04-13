/*
	Represents an element from a singly linked list.
*/

#ifndef __BOX_HEADER_INCLUDED__
#define __BOX_HEADER_INCLUDED__

template <class T>
struct Box
{
	Box(const T& item, Box<T>* next = nullptr) :
		item(item), next(next) { }

	Box<T>* next;
	T item;
};

#endif //__BOX_HEADER_INCLUDED__