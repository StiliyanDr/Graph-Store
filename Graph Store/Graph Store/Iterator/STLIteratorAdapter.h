#ifndef __STL_ITERATOR_ADAPTER_HEADER_INCLUDED__
#define __STL_ITERATOR_ADAPTER_HEADER_INCLUDED__

#include "Iterator.h"
#include <assert.h>

template <class STLIterator, bool isConst = false>
class STLIteratorAdapter : public AbstractIterator<typename STLIterator::value_type, isConst>
{
public:
	STLIteratorAdapter(STLIterator rangeStart, STLIterator rangeEnd);
	STLIteratorAdapter<STLIterator, isConst>& operator++() override;

protected:
	bool isValid() const override;

private:
	Reference getCurrentItem() const override;

private:
	STLIterator iterator;
	STLIterator end;
};

template <class STLConstIterator>
using STLConstIteratorAdapter = STLIteratorAdapter<STLConstIterator, true>;

template <class STLIterator, bool isConst>
STLIteratorAdapter<STLIterator, isConst>::STLIteratorAdapter(STLIterator rangeStart,
	                                                         STLIterator rangeEnd) :
	iterator(rangeStart),
	end(rangeEnd)
{
}

template <class STLIterator, bool isConst>
STLIteratorAdapter<STLIterator, isConst>&
STLIteratorAdapter<STLIterator, isConst>::operator++()
{
	if (isValid())
	{
		++iterator;
	}

	return *this;
}

template <class STLIterator, bool isConst>
inline bool STLIteratorAdapter<STLIterator, isConst>::isValid() const
{
	return iterator != end;
}

template <class STLIterator, bool isConst>
inline typename STLIteratorAdapter<STLIterator, isConst>::Reference
STLIteratorAdapter<STLIterator, isConst>::getCurrentItem() const
{
	assert(isValid());

	return *iterator;
}

#endif //__STL_ITERATOR_ADAPTER_HEADER_INCLUDED__