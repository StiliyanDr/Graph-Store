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

template <class Iterator, class UnaryFunction>
void forEach(Iterator& iterator, UnaryFunction f)
{
	while (iterator)
	{
		f(*iterator);

		++iterator;
	}
}

template <class Iterator, class UnaryPredicate>
inline bool allOf(Iterator& iterator, UnaryPredicate p)
{
	return !anyOf(iterator, unaryComplementOf(p));
}

template <class UnaryPredicate>
UnaryPredicate unaryComplementOf(UnaryPredicate p)
{
	return [p](auto&& x)
	{
		return !p(std::forward<decltype(x)>(x));
	};
}

template <class Iterator, class UnaryPredicate>
inline bool anyOf(Iterator& iterator, UnaryPredicate p)
{
	return static_cast<bool>(skipUntil(iterator, p));
}

template <class Iterator, class UnaryPredicate>
Iterator skipUntil(Iterator& iterator, UnaryPredicate p)
{
	while (iterator)
	{
		if (p(*iterator))
		{
			break;
		}

		++iterator;
	}

	return iterator;
}

#endif //__ITERATOR_HEADER_INCLUDED__