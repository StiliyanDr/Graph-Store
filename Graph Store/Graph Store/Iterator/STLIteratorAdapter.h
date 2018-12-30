#ifndef __STL_ITERATOR_ADAPTER_HEADER_INCLUDED__
#define __STL_ITERATOR_ADAPTER_HEADER_INCLUDED__

#include "Iterator.h"
#include <stdexcept>

template <class STLIterator, bool isConst = false>
class STLIteratorAdapter : public AbstractIterator<typename STLIterator::value_type, isConst>
{
public:
	STLIteratorAdapter(STLIterator rangeStart, STLIterator rangeEnd) :
		iterator(rangeStart),
		rangeEnd(rangeEnd)
	{
	}

	virtual STLIteratorAdapter<STLIterator, isConst>& operator++() override
	{
		if (isValid())
		{
			++iterator;
		}

		return *this;
	}

	virtual Reference operator*() const override
	{
		return getCurrentItem();
	}

	virtual Pointer operator->() const override
	{
		return &getCurrentItem();
	}
	
	virtual bool operator!() const noexcept override
	{
		return !isValid();
	}
	
	virtual operator bool() const noexcept override
	{
		return isValid();
	}

private:
	Reference getCurrentItem() const
	{
		if (isValid())
		{
			return *iterator;
		}
		else
		{
			throw std::out_of_range("Iterator out of range!");
		}
	}

	bool isValid() const noexcept
	{
		return iterator != rangeEnd;
	}

private:
	STLIterator iterator;
	STLIterator rangeEnd;
};

template <class STLConstIterator>
using STLConstIteratorAdapter = STLIteratorAdapter<STLConstIterator, true>;

#endif //__STL_ITERATOR_ADAPTER_HEADER_INCLUDED__