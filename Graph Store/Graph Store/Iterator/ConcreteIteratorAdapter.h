#ifndef __CONCRETE_ITERATOR_ADAPTER_HEADER_INCLUDED__
#define __CONCRETE_ITERATOR_ADAPTER_HEADER_INCLUDED__

#include "Iterator.h"

template <class Item, class ConcreteIterator, bool isConst = false>
class ConcreteIteratorAdapter : public Iterator<Item, isConst>
{
public:
	ConcreteIteratorAdapter(const ConcreteIterator& iterator) :
		iterator(iterator)
	{
	}

	virtual Iterator<Item>& operator++() override
	{
		++iterator;
	}

	virtual Reference operator*() const override
	{
		return *iterator;
	}
	
	virtual Pointer operator->() const override
	{
		return iterator.operator->();
	}
	
	virtual bool operator!() const override
	{
		return !iterator;
	}
	
	virtual operator bool() const override
	{
		return static_cast<bool>(iterator);
	}

private:
	ConcreteIterator iterator;
};

#endif //__CONCRETE_ITERATOR_ADAPTER_HEADER_INCLUDED__