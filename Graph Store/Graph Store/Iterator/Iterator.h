#ifndef __ITERATOR_HEADER_INCLUDED__
#define __ITERATOR_HEADER_INCLUDED__

#include <type_traits>

template <class Item, bool isConst = false>
class AbstractIterator
{
public:
	using Reference = typename std::conditional<isConst, const Item&, Item&>::type;
	using Pointer = typename std::conditional<isConst, const Item*, Item*>::type;

public:
	virtual ~AbstractIterator() = default;

	virtual AbstractIterator<Item, isConst>& operator++() = 0;

	Reference operator*() const
	{
		return verifyValidityAndReturnCurrentItem();
	}
	
	Pointer operator->() const
	{
		return std::addressof(verifyValidityAndReturnCurrentItem());
	}

	bool operator!() const
	{
		return !isValid();
	}
	
	operator bool() const
	{
		return isValid();
	}

protected:
	AbstractIterator() = default;
	AbstractIterator(const AbstractIterator<Item, isConst>&) = default;
	AbstractIterator&
		operator=(const AbstractIterator<Item, isConst>&) = default;
	
	virtual bool isValid() const = 0;

private:
	Reference verifyValidityAndReturnCurrentItem() const;
	virtual Reference getCurrentItem() const = 0;
};

template <class Item>
using Iterator = AbstractIterator<Item, false>;

template <class Item>
using ConstIterator = AbstractIterator<Item, true>;

template <class Item, bool isConst>
typename AbstractIterator<Item, isConst>::Reference
AbstractIterator<Item, isConst>::verifyValidityAndReturnCurrentItem() const
{
	if (isValid())
	{
		return getCurrentItem();
	}
	else
	{
		throw std::out_of_range("Iterator out of range!");
	}
}

template <class Iterator, class UnaryPredicate, class UnaryFunction>
inline void forEachUntil(Iterator& iterator,
	                     UnaryPredicate p,
	                     UnaryFunction f)
{
	forEachWhile(iterator, unaryComplementOf(p), f);
}

template <class Iterator, class UnaryFunction>
void forEach(Iterator& iterator, UnaryFunction f)
{
	auto alwaysTrue =
		[](const auto&) noexcept { return true; };
	forEachWhile(iterator, alwaysTrue, f);
}

template <class Iterator, class UnaryPredicate, class UnaryFunction>
void forEachWhile(Iterator& iterator, UnaryPredicate p, UnaryFunction f)
{
	while (iterator)
	{
		auto&& item = *iterator;

		if (!p(item))
		{
			break;
		}

		using T = decltype(item);
		f(std::forward<T>(item));

		++iterator;
	}
}

template <class Iterator, class UnaryPredicate>
inline bool allOf(Iterator iterator, UnaryPredicate p)
{
	return !anyOf(iterator, unaryComplementOf(p));
}

template <class UnaryPredicate>
auto unaryComplementOf(UnaryPredicate p)
{
	return [p](auto&& x)
	{
		return !p(std::forward<decltype(x)>(x));
	};
}

template <class Iterator, class UnaryPredicate>
inline bool anyOf(Iterator iterator, UnaryPredicate p)
{
	return static_cast<bool>(findIf(iterator, p));
}

template <class Iterator, class UnaryPredicate>
Iterator findIf(Iterator iterator, UnaryPredicate p)
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

template <class Iterator, class UnaryPredicate>
inline bool noneOf(Iterator iterator, UnaryPredicate p)
{
	return !anyOf(iterator, p);
}

#endif //__ITERATOR_HEADER_INCLUDED__