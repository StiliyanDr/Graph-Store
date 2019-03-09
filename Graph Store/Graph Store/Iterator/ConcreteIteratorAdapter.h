#ifndef __CONCRETE_ITERATOR_ADAPTER_HEADER_INCLUDED__
#define __CONCRETE_ITERATOR_ADAPTER_HEADER_INCLUDED__

#include "Iterator.h"

template <class Item, class ConcreteIterator, bool isConst = false>
class ConcreteIteratorAdapter : public AbstractIterator<Item, isConst>
{
public:
	ConcreteIteratorAdapter(ConcreteIterator iterator) :
		iterator(iterator)
	{
	}

	ConcreteIteratorAdapter<Item, ConcreteIterator, isConst>&
	operator++() override
	{
		++iterator;

		return *this;
	}

protected:
	bool isValid() const override
	{
		return static_cast<bool>(iterator);
	}

private:
	Reference getCurrentItem() const override
	{
		assert(isValid());

		return *iterator;
	}

private:
	ConcreteIterator iterator;
};

template <class Item, class ConcreteIterator>
using ConcreteConstIteratorAdapter =
	ConcreteIteratorAdapter<Item, ConcreteIterator, true>;

#endif //__CONCRETE_ITERATOR_ADAPTER_HEADER_INCLUDED__