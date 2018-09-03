#ifndef __ITERATOR_HEADER_INCLUDED__
#define __ITERATOR_HEADER_INCLUDED__

#include <type_traits>

template <class Item, bool isConst = false>
class Iterator
{
protected:
	typedef typename std::conditional<isConst, const Item&, Item&>::type Reference;
	typedef typename std::conditional<isConst, const Item*, Item*>::type Pointer;

public:
	virtual ~Iterator() = default;

	virtual Iterator<Item>& operator++() = 0;
	virtual Reference operator*() const = 0;
	virtual Pointer operator->() const = 0;
	virtual bool operator!() const = 0;
	virtual operator bool() const = 0;

protected:
	Iterator() = default;
};

template <class Iterator, class Function>
void forEach(Iterator& iterator, const Function& function)
{
	while (iterator)
	{
		function(*iterator);

		++iterator;
	}
}

#endif //__ITERATOR_HEADER_INCLUDED__