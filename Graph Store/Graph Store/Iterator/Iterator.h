#ifndef __ITERATOR_HEADER_INCLUDED__
#define __ITERATOR_HEADER_INCLUDED__

#include <type_traits>

template <class Item, bool isConst = false>
class AbstractIterator
{
public:
	typedef typename std::conditional<isConst, const Item&, Item&>::type Reference;
	typedef typename std::conditional<isConst, const Item*, Item*>::type Pointer;

public:
	virtual ~AbstractIterator() = default;

	virtual AbstractIterator<Item, isConst>& operator++() = 0;
	virtual Reference operator*() const = 0;
	virtual Pointer operator->() const = 0;
	virtual bool operator!() const = 0;
	virtual operator bool() const = 0;

protected:
	AbstractIterator() = default;
};

template <class Item>
using Iterator = AbstractIterator<Item, false>;

template <class Item>
using ConstIterator = AbstractIterator<Item, true>;

template <class Iterator, class Function>
void forEach(Iterator& iterator, const Function& function)
{
	while (iterator)
	{
		function(*iterator);

		++iterator;
	}
}

template <class Iterator, class Predicate>
Iterator forEachUntil(Iterator& iterator, const Predicate& predicate)
{
	while (iterator)
	{
		if (predicate(*iterator))
		{
			break;
		}

		++iterator;
	}

	return iterator;
}

#endif //__ITERATOR_HEADER_INCLUDED__